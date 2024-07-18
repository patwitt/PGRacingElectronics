/*
 * RPMTables.h
 *
 *  Created on: 12.07.2022
 *      Author: Patryk Wittbrodt
 */

#ifndef SRC_APPLICATION_GEARCONTROL_SRC_RPM_TABLE_H_
#define SRC_APPLICATION_GEARCONTROL_SRC_RPM_TABLE_H_

#include "Types.h"
#include "Utils.h"

#define BASE_RATIO (1.848f)               //!< Base Gear Ratio
#define TRANSMISSION_RATIO (0.255813953f) //!< Transmission Ratio

#define RPM_TABLE_LENGTH (13U)

#define WHEEL_RPM_MIN (59.0f)
#define WHEEL_RPM_MAX (1600.0f)


#define GEAR_1_TRANSMISSION_RATIO (2.3125f)
#define GEAR_2_TRANSMISSION_RATIO (1.857f)
#define GEAR_3_TRANSMISSION_RATIO (1.565f)
#define GEAR_4_TRANSMISSION_RATIO (1.35f)
#define GEAR_5_TRANSMISSION_RATIO (1.238f)
#define GEAR_6_TRANSMISSION_RATIO (1.136f)

#define DOWNSHIFT_RPM_G1_MULTIPLIER (GEAR_1_TRANSMISSION_RATIO / GEAR_2_TRANSMISSION_RATIO) //!< 124.53%
#define DOWNSHIFT_RPM_G2_MULTIPLIER (GEAR_2_TRANSMISSION_RATIO / GEAR_3_TRANSMISSION_RATIO) //!< 118.69%
#define DOWNSHIFT_RPM_G3_MULTIPLIER (GEAR_3_TRANSMISSION_RATIO / GEAR_4_TRANSMISSION_RATIO) //!< 115.93%
#define DOWNSHIFT_RPM_G4_MULTIPLIER (GEAR_4_TRANSMISSION_RATIO / GEAR_5_TRANSMISSION_RATIO) //!< 109.05%
#define DOWNSHIFT_RPM_G5_MULTIPLIER (GEAR_5_TRANSMISSION_RATIO / GEAR_6_TRANSMISSION_RATIO) //!< 108.98%


#define RPM_THROTTLE_LUT_CNT (13U)

//! Engine RPM Lookup Table X values
static const float X_Rpm[RPM_THROTTLE_LUT_CNT] = {
	1000.0f,
	2000.0f,
	3000.0f,
	4000.0f,
	5000.0f,
	6000.0f,
	7000.0f,
	8000.0f,
	9000.0f,
	10000.0f,
	11000.0f,
	12000.0f,
	13000.0f
};

//! Gear 3 -> 2 Throttle Degrees Map 0-1000 [0-100%]
static const float Y_Gear2ThrottleMap[RPM_THROTTLE_LUT_CNT] = {
	300.0f,
	300.0f,
	300.0f,
	300.0f,
	300.0f,
	300.0f,
	300.0f,
	300.0f,
	300.0f,
	300.0f,
	300.0f,
	300.0f,
	300.0f
};

//! Gear 4 -> 3 Throttle Degrees Map 0-1000 [0-100%]
static const float Y_Gear3ThrottleMap[RPM_THROTTLE_LUT_CNT] = {
	300.0f,
	300.0f,
	300.0f,
	300.0f,
	300.0f,
	300.0f,
	300.0f,
	300.0f,
	300.0f,
	300.0f,
	300.0f,
	300.0f,
	300.0f
};

//! Gear 5 -> 4 Throttle Degrees Map 0-1000 [0-100%]
static const float Y_Gear4ThrottleMap[RPM_THROTTLE_LUT_CNT] = {
	300.0f,
	300.0f,
	300.0f,
	300.0f,
	300.0f,
	300.0f,
	300.0f,
	300.0f,
	300.0f,
	300.0f,
	300.0f,
	300.0f,
	300.0f
};

//! Gear 6 -> 5 Throttle Degrees Map 0-1000 [0-100%]
static const float Y_Gear5ThrottleMap[RPM_THROTTLE_LUT_CNT] = {
	300.0f,
	300.0f,
	300.0f,
	300.0f,
	300.0f,
	300.0f,
	300.0f,
	300.0f,
	300.0f,
	300.0f,
	300.0f,
	300.0f,
	300.0f
};


#if REVMATCH_FROM_WHEEL_RPM
static const float EngineRPM_Y[RPM_TABLE_LENGTH] = {
	1000.0f,
	2000.0f,
	3000.0f,
	4000.0f,
	5000.0f,
	6000.0f,
	7000.0f,
	8000.0f,
	9000.0f,
	10000.0f,
	11000.0f,
	12000.0f,
	13000.0f
};

static const float G1_WheelRPM_X[RPM_TABLE_LENGTH] = {
	59.86052498f,
	119.72105f,
	179.5815749f,
	239.4420999f,
	299.3026249f,
	359.1631499f,
	419.0236748f,
	478.8841998f,
	538.7447248f,
	598.6052498f,
	658.4657747f,
	718.3262997f,
	778.1868247f
};

static const float G2_WheelRPM_X[RPM_TABLE_LENGTH] = {
	74.54359936f,
	149.0871987f,
	223.6307981f,
	298.1743974f,
	372.7179968f,
	447.2615962f,
	521.8051955f,
	596.3487949f,
	670.8923942f,
	745.4359936f,
	819.9795929f,
	894.5231923f,
	969.0667917f
};

static const float G3_WheelRPM_X[RPM_TABLE_LENGTH] = {
	88.45205368f,
	176.9041074f,
	265.356161f,
	353.8082147f,
	442.2602684f,
	530.7123221f,
	619.1643758f,
	707.6164294f,
	796.0684831f,
	884.5205368f,
	972.9725905f,
	1061.424644f,
	1149.876698f
};

static const float G4_WheelRPM_X[RPM_TABLE_LENGTH] = {
	102.5388622f,
	205.0777245f,
	307.6165867f,
	410.1554489f,
	512.6943111f,
	615.2331734f,
	717.7720356f,
	820.3108978f,
	922.8497601f,
	1025.388622f,
	1127.927485f,
	1230.466347f,
	1333.005209f
};

static const float G5_WheelRPM_X[RPM_TABLE_LENGTH] = {
	111.815399f,
	223.6307981f,
	335.4461971f,
	447.2615962f,
	559.0769952f,
	670.8923942f,
	782.7077933f,
	894.5231923f,
	1006.338591f,
	1118.15399f,
	1229.969389f,
	1341.784788f,
	1453.600187f
};

static const float G6_WheelRPM_X[RPM_TABLE_LENGTH] = {
	121.855162f,
	243.710324f,
	365.5654859f,
	487.4206479f,
	609.2758099f,
	731.1309719f,
	852.9861339f,
	974.8412958f,
	1096.696458f,
	1218.55162f,
	1340.406782f,
	1462.261944f,
	1584.117106f
};

static const table_1d G1_rpmMap = {
	.x_length = RPM_TABLE_LENGTH,
	.x_values = G1_WheelRPM_X,
	.y_values = EngineRPM_Y
};

static const table_1d G2_rpmMap = {
	.x_length = RPM_TABLE_LENGTH,
	.x_values = G2_WheelRPM_X,
	.y_values = EngineRPM_Y
};

static const table_1d G3_rpmMap = {
	.x_length = RPM_TABLE_LENGTH,
	.x_values = G3_WheelRPM_X,
	.y_values = EngineRPM_Y
};

static const table_1d G4_rpmMap = {
	.x_length = RPM_TABLE_LENGTH,
	.x_values = G4_WheelRPM_X,
	.y_values = EngineRPM_Y
};

static const table_1d G5_rpmMap = {
	.x_length = RPM_TABLE_LENGTH,
	.x_values = G5_WheelRPM_X,
	.y_values = EngineRPM_Y
};

static const table_1d G6_rpmMap = {
	.x_length = RPM_TABLE_LENGTH,
	.x_values = G6_WheelRPM_X,
	.y_values = EngineRPM_Y
};
#endif  // REVMATCH_FROM_WHEEL_RPM
#endif /* SRC_APPLICATION_GEARCONTROL_SRC_RPM_TABLE_H_ */
