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

//! A lookup table for the gear requests.
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
/*        Local functions       */
/* ---------------------------- */

/**
 * @brief Validation of the new gear request.
 * 
 * If the current gear is 1, then the only valid request is to shift up. If the current gear is 6,
 * then the only valid request is to shift up. If the current gear is N, then the only valid requests
 * are to shift up or down. If the current gear is 2, 3, 4, or 5, then the only valid requests are to
 * shift up or down
 * 
 * @param request The requested gear change.
 * 
 * @return Gear request that will be processed.
 */
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

		case GEAR_BYPASS:
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
					ret = GEAR_REQUEST_INVALID;
					break;

				case GEAR_REQUEST_SHIFT_DOWN:
					ret = request;
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
/**
 * @brief Initialization of the gear request module.
 * 
 * @return an error code.
 */
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

/**
 * @brief Get current gear request.
 * 
 * If the microswitch is enabled, then return the gear request based on the state of the microswitches.
 * The first thing we do is set the request to GEAR_REQUEST_NONE. This is the default value.
 * Next, we check if the microswitch is enabled. If it is, then we get the gear request from the LUT.
 * The LUT is a 2D array. The first dimension is the state of the gear down microswitch. The second
 * dimension is the state of the gear up microswitch.
 * 
 * @return Gear request that will be processed.
 */
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

