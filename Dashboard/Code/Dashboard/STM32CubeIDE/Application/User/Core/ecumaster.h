/*
 * ecumaster.h
 *
 *  Created on: May 20, 2022
 *      Author: Marcin Nadarzyński
 */

#ifndef APPLICATION_USER_CORE_ECUMASTER_H_
#define APPLICATION_USER_CORE_ECUMASTER_H_

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct EcumasterData_t
{
	uint16_t rpm; 						// Engine RPM
	uint8_t tps;						// Throttle position
	uint8_t iat;						// Intake manifold temperature
	uint16_t map;						// Manifold absolute pressure
	uint16_t injPW;						// Injection pulse width
	uint16_t speed;						// Vehicle speed
	uint8_t baro;						// Barometric pressure
	uint8_t oilTemp;					// Oil temperature
	float oilPress;					// Oil pressure
	int16_t clt;						// Engine coolant temperature
	int8_t ignAngle;					// Ignition advance
	uint8_t ignDwell;					// Ignition coil dwell
	uint8_t lambda;						//
	uint8_t lambdaCorrection;			//
	uint16_t egt1;						// Exhaust gas temperature #1
	uint16_t egt2;						// Exhaust gas temperature #2
	uint8_t gear;						//
	int8_t ecuTemp;						// ECU internal temperature
	float batt;						// Battery voltage
	uint16_t errflag;					//
	uint8_t flags1;						//
	uint8_t DBWPosition;				//
	uint8_t DBWTrigger;					//
	uint16_t TCDRPMRaw;					// Traction Control Delta RPM Raw
	uint8_t TCDRPM;						// Traction Control Delta RPM
	uint8_t TCTorqueReduction;			//
	uint8_t PitLimitTorqueReduction;	//
	float ain1;
	float ain2;
	float ain3;
	float ain4;
} EcumasterData;

enum EcumasterFrame
{
	Frame1 = 0x600, Frame2, Frame3, Frame4, Frame5, Frame6, Frame7, Frame8
};

extern EcumasterData EcuData;

void validateData();

#ifdef __cplusplus
}
#endif

#endif /* APPLICATION_USER_CORE_ECUMASTER_H_ */
