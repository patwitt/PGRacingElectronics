/*
 * DBWUtils.h
 *
 *  Created on: 7 lip 2024
 *      Author: Patryk Wittbrodt
 */

#ifndef SRC_APPLICATION_DBW_SRC_DBWUTILS_H_
#define SRC_APPLICATION_DBW_SRC_DBWUTILS_H_

#include "DBWTypes.h"
#include "DCMotor.h"

/**
 * @brief Disable Drive-By-Wire.
 *
 * If the DBW is enabled, disable it and set the state to disabled.
 */
static inline void DBW_Disable(DbwHandle *const dbw_)
{
	/* Disable DBW */
	dbw_->state = DBW_DISABLED;
	DCMotor_Disable();
}

/**
 * @brief Convert the raw TPS value to a percentage value between 0 and 1000.
 *
 * The first thing we do is set the default value of tpsPos to TPS_POS_MIN_F. This is the default value
 * of 0.0.
 *
 * @return Coverted TPS raw value to float.
 */
static inline float DBWUtils_ConvertTpsRaw(TpsSensorType *const tps_)
{
	float tpsPos = TPS_POS_MIN_F;

	if (tps_->tps2->avgData.avg >= tps_->limits->min) {
		tpsPos = ((float)tps_->tps2->avgData.avg - tps_->limits->min) * TPS_DIVISOR_F(tps_->limits->min, tps_->limits->max);
		tpsPos = CLAMP_MAX(tpsPos, TPS_POS_MAX_F);
	}

	return tpsPos;
}

/**
 * @brief Convert the raw APPS value to a percentage value between 0 and 1000.
 *
 * The first thing we do is set the default value of targetApps to TPS_POS_MIN_F. This is the default value
 * of 0.0.
 *
 * @return Coverted APPS raw value to float.
 */
static inline float DBWUtils_ConvertAppsRaw(AppsSensorType *const apps_)
{
	float targetApps = APPS_POS_MIN_F;

	if (apps_->apps2->avgData.avg >= apps_->limits->min) {
		targetApps = ((float)apps_->apps2->avgData.avg - apps_->limits->min) * APPS_DIVISOR_F(apps_->limits->min, apps_->limits->max);
		targetApps = CLAMP_MAX(targetApps, APPS_POS_MAX_F);
	}

	return targetApps;
}

static inline bool_t DBWUtils_IsIdle(TpsSensorType *const tps_)
{
#define TPS_IDLE_MARK (TPS_IDLE + 50.0f) // IDLE + 5%

	return (DBWUtils_ConvertTpsRaw(tps_) <= TPS_IDLE_MARK);
}

#endif /* SRC_APPLICATION_DBW_SRC_DBWUTILS_H_ */
