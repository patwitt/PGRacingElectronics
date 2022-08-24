/*
 * TelemetryData.h
 *
 *  Created on: Aug 10, 2022
 *      Author: Marcin
 */

#ifndef APPLICATION_USER_CORE_TELEMETRY_DATA_H_
#define APPLICATION_USER_CORE_TELEMETRY_DATA_H_


#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
 uint8_t gear;
 uint8_t steeringWheelAttached;
 uint32_t steeringWheelAttachedTimestamp;
 uint8_t isIntercomActive;
 uint8_t oilPress;
 //float fuelConsumption;
 float burnedFuel;
}telemetryData_t;

extern telemetryData_t telemetryData;

#ifdef __cplusplus
}
#endif


#endif /* APPLICATION_USER_CORE_TELEMETRY_DATA_H_ */
