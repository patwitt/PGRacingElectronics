/*
 * transmitter.h
 *
 *  Created on: 20 sie 2022
 *      Author: Patryk
 */

#ifndef INC_LOGGER_TRANSMITTER_H_
#define INC_LOGGER_TRANSMITTER_H_
#include "ecumaster.h"
typedef struct {
	union {
		uint32_t flags;
		struct{
			uint8_t rpmOn : 1;						// Engine RPM
			uint8_t tpsOn: 1;						// Throttle position
			uint8_t iatOn: 1;						// Intake manifold temperature
			uint8_t mapOn: 1;						// Manifold absolute pressure
			uint8_t injPWOn: 1;						// Injection pulse width
			uint8_t speedOn: 1;						// Vehicle speed
			uint8_t baroOn: 1;						// Barometric pressure
			uint8_t oilTempOn: 1;					// Oil temperature
			uint8_t oilPressOn: 1;					// Oil pressure
			uint8_t cltOn: 1;						// Engine coolant temperature
			uint8_t ignAngleOn: 1;					// Ignition advance
			uint8_t ignDwellOn: 1;					// Ignition coil dwell
			uint8_t lambdaOn: 1;						//
			uint8_t lambdaCorrectionOn: 1;			//
			uint8_t egt1On: 1;						// Exhaust gas temperature #1
			uint8_t egt2On: 1;						// Exhaust gas temperature #2
			uint8_t gearOn : 1;						//
			uint8_t ecuTempOn: 1;						// ECU internal temperature
			uint8_t battOn : 1;						// Battery voltage
			uint8_t errflagOn: 1;					//
			uint8_t flags1On: 1;						//
			uint8_t DBWPositionOn: 1;				//
			uint8_t DBWTriggerOn: 1;					//
			uint8_t TCDRPMRawOn: 1;					// Traction Control Delta RPM Raw
			uint8_t TCDRPMOn: 1;						// Traction Control Delta RPM
			uint8_t TCTorqueReductionOn: 1;			//
			uint8_t PitLimitTorqueReductionOn: 1;	//
			uint8_t BurnedFuelOn : 1;
		};
	};


}ecuLogRegister;

void parseCommand(char * command);
void sendEcuLogs(EcumasterData EcuData);


#endif /* INC_LOGGER_TRANSMITTER_H_ */
