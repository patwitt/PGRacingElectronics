/*
 * GearControlMap.h
 *
 *  Created on: 12.07.2022
 *      Author: Patryk Wittbrodt
 */

#ifndef SRC_APPLICATION_GEARCONTROL_SRC_GEARCONTROL_MAP_H_
#define SRC_APPLICATION_GEARCONTROL_SRC_GEARCONTROL_MAP_H_

#include "Types.h"

typedef struct {
	const uint32_t degreesDown;
	const uint32_t degreesUp;
	const uint32_t degreesN;
	const GearStates expGearDown;
	const GearStates expGearUp;
} GearServoDegData;

/* Degrees values wrapped in macros for mapping */
/* Servo degrees limits */
#define DEG_DEFAULT    (99U)         //!< Default position
#define DEG_UP_MAX     (80U)         //!< Degrees UP max. in relation to DEFAULT pos
#define DEG_DOWN_MAX   (80U)         //!< Degrees DOWN max. in relation to DEFAULT pos
#define DEG_PROHIBITED (DEG_DEFAULT) //!< Prohibited action e.g. 6 -> UP. Will just stay in DEFAULT position

/* Servo degrees values */
/* UPSHIFTS */
#define DEG_1_2_UP (DEG_DEFAULT + DEG_UP_MAX)   //!< Degrees for up-shift 1 -> 2
#define DEG_UP     (DEG_DEFAULT + 78U)          //!< Degrees for up-shifts (gears 2, 3, 4, 5)
#define DEG_N_2_UP (DEG_DEFAULT + 50U)          //!< Degrees for up-shift N -> 2
#define DEG_1_N    (DEG_DEFAULT + 33U)          //!< Degrees for shift 1 -> N @NOTE: !This is only option for going to NEUTRAL (DOWNSHIFT)!

/* DOWNSHIFTS */
#define DEG_2_1_D  (DEG_DEFAULT - DEG_DOWN_MAX) //!< Degrees for down-shift 2 -> 1
#define DEG_DOWN   (DEG_DEFAULT - 78U)          //!< Degrees for down-shifts (gears 3, 4, 5, 6)
#define DEG_N_1_D  (DEG_DEFAULT - 50U)          //!< Degrees for down-shift N -> 1

static const GearServoDegData GearShiftDegMap[GEAR_COUNT] = {
     /* --------------------------------------------------------------------------------------------- */
	 /* ------------------------------ SERVO DEGREES TO GEARS MAPPING ------------------------------- */
	 /* --------------------------------------------------------------------------------------------- */
	 /* GEAR   |   DOWN[deg]     |     UP[deg]     |  NEUTRAL[deg]   |   Exp G Down   |    Exp G Up   */
	 /* --------------------------------------------------------------------------------------------- */
	 /* G 1    |     DEG NONE    |   1 -> 2        |      1 -> N     |  GEAR NONE     |     1 -> 2    */
	 [GEAR_1] = { DEG_PROHIBITED,   DEG_1_2_UP,          DEG_1_N,      GEAR_DISABLED,       GEAR_2    },
	 /* G N    |     N -> 1      |   N -> 2        |     DEG NONE    |    N -> 1      |     N -> 2    */
	 [GEAR_N] = {   DEG_N_1_D,      DEG_N_2_UP,      DEG_PROHIBITED,      GEAR_1,           GEAR_2    },
	 /* G 2    |     2 -> 1      |   2 -> 3        |     DEG NONE    |    2 -> 1      |     2 -> 3    */
	 [GEAR_2] = {   DEG_2_1_D,       DEG_UP,         DEG_PROHIBITED,      GEAR_1,           GEAR_3    },
	 /* G 3    |     3 -> 2      |   3 -> 4        |     DEG NONE    |    3 -> 2      |     3 -> 4    */
	 [GEAR_3] = {   DEG_DOWN,        DEG_UP,         DEG_PROHIBITED,      GEAR_2,           GEAR_4    },
	 /* G 4    |     4 -> 3      |   4 -> 5        |     DEG NONE    |    4 -> 3      |     3 -> 4    */
	 [GEAR_4] = {   DEG_DOWN,        DEG_UP,         DEG_PROHIBITED,      GEAR_3,           GEAR_5    },
	 /* G 5    |     5 -> 4      |   5 -> 6        |     DEG NONE    |    5 -> 4      |     3 -> 4    */
	 [GEAR_5] = {   DEG_DOWN,        DEG_UP,         DEG_PROHIBITED,      GEAR_4,           GEAR_6    },
	 /* G 6    |     6 -> 5      |    NONE         |     DEG NONE    |    6 -> 5      |   GEAR NONE   */
	 [GEAR_6] = {   DEG_DOWN,      DEG_PROHIBITED,   DEG_PROHIBITED,      GEAR_5,       GEAR_DISABLED },
     /* --------------------------------------------------------------------------------------------- */
	 /* --------------------------------- ANYTHING ELSE IS INVALID ---------------------------------- */
	 [GEAR_INIT]     = { DEG_PROHIBITED, DEG_PROHIBITED, DEG_PROHIBITED ,GEAR_DISABLED, GEAR_DISABLED },
	 [GEAR_DISABLED] = { DEG_PROHIBITED, DEG_PROHIBITED, DEG_PROHIBITED ,GEAR_DISABLED, GEAR_DISABLED }
};

#endif /* SRC_APPLICATION_GEARCONTROL_SRC_GEARCONTROL_MAP_H_ */
