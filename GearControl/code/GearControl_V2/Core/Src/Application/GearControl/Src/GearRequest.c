/*
 * GearRequest.c
 *
 *  Created on: 19.06.2022
 *      Author: Patryk Wittbrodt
 */
#include "Types.h"
#include "MicroSwitch.h"
#include "GearRequest.h"
#include "GearControl.h"

/* ---------------------------- */
/*         Local data           */
/* ---------------------------- */
static __IO MicroSwitch* msGearUp = NULL;
static __IO MicroSwitch* msGearDown = NULL;

static const GearRequestEnum gearRequestLUT[MS_STATE_COUNT + 1U][MS_STATE_COUNT + 1U] = {
		//   LOW LOW             LOW HIGH         LOW DEBOUNCING      LOW COUNT
		{GEAR_REQUEST_NONE, GEAR_REQUEST_SHIFT_UP, GEAR_REQUEST_NONE, GEAR_REQUEST_INVALID},   // G DOWN LOW
		//   HIGH LOW                HIGH  HIGH       HIGH DEBOUNCING    HIGH COUNT
		{GEAR_REQUEST_SHIFT_DOWN, GEAR_REQUEST_NONE, GEAR_REQUEST_NONE, GEAR_REQUEST_INVALID}, // G DOWN HIGH
		// DEBOUNCING LOW   DEBOUNCING  HIGH  DEBOUNCING DEBOUNCING  DEBOUNCING COUNT
		{GEAR_REQUEST_NONE, GEAR_REQUEST_NONE, GEAR_REQUEST_NONE, GEAR_REQUEST_INVALID}, // G DOWN DEBOUNCING
		//  COUNT LOW             COUNT  HIGH          COUNT DEBOUNCING       COUNT COUNT
		{GEAR_REQUEST_INVALID, GEAR_REQUEST_INVALID, GEAR_REQUEST_INVALID, GEAR_REQUEST_INVALID}  // G DOWN COUNT
};

/* ---------------------------- */
/* Local function declarations  */
/* ---------------------------- */
static inline GearRequestEnum GearRequest_Validate(const GearRequestEnum request);

/* ---------------------------- */
/*       Static functions       */
/* ---------------------------- */
static inline GearRequestEnum GearRequest_Validate(const GearRequestEnum request)
{
	GearRequestEnum ret = GEAR_REQUEST_NONE;
	const GearStates currentGear = GearControl_GetGear();

	switch (currentGear) {
		case GEAR_1:
			switch (request) {
				case GEAR_REQUEST_SHIFT_UP:
					ret = request;
					break;

				case GEAR_REQUEST_SHIFT_DOWN:
					ret = GEAR_REQUEST_SHIFT_N;
					break;

				default:
					break;
			}
			break;

		case GEAR_N:
		case GEAR_2:
		case GEAR_3:
		case GEAR_4:
		case GEAR_5:
			switch (request) {
				case GEAR_REQUEST_SHIFT_UP:
				case GEAR_REQUEST_SHIFT_DOWN:
					ret = request;
					break;
				default:

					break;
			}
			break;

		case GEAR_6:
			switch (request) {
				case GEAR_REQUEST_SHIFT_UP:
					ret = request;
					break;

				case GEAR_REQUEST_SHIFT_DOWN:
					ret = GEAR_REQUEST_INVALID;
					break;

				default:
					break;
			}

		default:
			break;
	}

	return ret;
}

/* ---------------------------- */
/*       Global functions       */
/* ---------------------------- */
ErrorEnum GearRequest_Init(void)
{
	ErrorEnum err = ERROR_OK;

	MicroSwitch_Init();

	msGearUp = MicroSwitch_Get(MS_G_UP);
	msGearDown = MicroSwitch_Get(MS_G_DOWN);

	if ((msGearUp == NULL) || (msGearDown == NULL)) {
		err = ERROR_NULL;
	}

	return err;
}

GearRequestEnum GearRequest_Get(void)
{
	GearRequestEnum request = GEAR_REQUEST_NONE;

	if (MicroSwitch_GetControl() == MS_CONTROL_ENABLED) {
		request = gearRequestLUT[msGearDown->state][msGearUp->state];

		/* Validate only SHIFT UP and SHIFT DOWN, anything else is invalid */
		switch (request) {
			case GEAR_REQUEST_SHIFT_UP:
			case GEAR_REQUEST_SHIFT_DOWN:
				request = GearRequest_Validate(request);
				break;

			default:
				break;

		}
	}

	return request;
}

