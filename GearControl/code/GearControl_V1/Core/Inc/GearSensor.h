/*
 * GearSensor.h
 *
 *  Created on: 22.05.2021
 *      Author: User
 */

#ifndef INC_GEARSENSOR_H_
#define INC_GEARSENSOR_H_

#define N_SAMPLES (4U)
#define SAMPLES_3 (3U)
#define SAMPLES_2 (2U)
#define N_CHANNELS (4U)
#define CHECK_IN_PROG (3U)
#define OVERCURRENT_THRESHOLD (3100U)
#define GEARSENSOR_UNDEFINED (0U)

void GearSensor_Process(void);
void GearSensor_DMAProcess(void);
GearStates GearSensor_GetStateBySensorAdc(uint8 nSamples);
void GearSensor_Init(__IO uint16* const);

#endif /* INC_GEARSENSOR_H_ */
