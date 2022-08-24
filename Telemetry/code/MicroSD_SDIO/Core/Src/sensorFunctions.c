/*
 * sensorFunctions.h
 *
 *  Created on: Jun 17, 2022
 *      Author: Patryk
 */

//INCLUDES

#include "sensorFunctions.h"
#include <stdio.h>
#include <math.h>
extern RTC_HandleTypeDef hrtc;
extern SensorStatus statusRegister;
extern UART_HandleTypeDef huart7;

/* *******GLOBAL VARIABLES ********/
MLXSensor mlxLFSensor;
MLXSensor mlxRFSensor;
GyroSensor gyro;
ABSSensor absLFSensor;
ABSSensor absRFSensor;
GPSSensor gpsSensor;
//GPS
char commands[4][255] = {"$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29\r\n",//Send Data setup
		"$PMTK251,115200*1f\r\n", //BuadRate change
		"$PMTK869,1,0*34\r\n", // Easy mode off
		"$PMTK220,100*2f\r\n"}; // Update Rate



int calculateCSM(char * command)
{
	int chsm = 0;
	char c;
	for(int i=0; i < strlen(command); i++)
	{
		c = command[i];
		chsm = chsm ^ c;
	}
	return chsm;
}

void createCommand(char * command)
{
	char final[255];
	int chsm = calculateCSM(command);
	sprintf(final, "$%s*%02X\r\n", command, chsm);
	strcpy(command,final);
}

static void gpsReinitUart(GPSSensor * sens)
{
  HAL_UART_Abort_IT(sens->uart);
  HAL_UART_DeInit(sens->uart);
  huart7.Instance = UART7;
  huart7.Init.BaudRate = 115200;
  huart7.Init.WordLength = UART_WORDLENGTH_8B;
  huart7.Init.StopBits = UART_STOPBITS_1;
  huart7.Init.Parity = UART_PARITY_NONE;
  huart7.Init.Mode = UART_MODE_TX_RX;
  huart7.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart7.Init.OverSampling = UART_OVERSAMPLING_16;
  huart7.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart7.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_RXOVERRUNDISABLE_INIT;
  huart7.AdvancedInit.OverrunDisable = UART_ADVFEATURE_OVERRUN_DISABLE;
  if (HAL_UART_Init(sens->uart) != HAL_OK)
  {
	  Error_Handler();
  }


}


extern UART_HandleTypeDef huart3;
void GPSInit(GPSSensor * sens)
{
	sens->File = (FIL*)malloc(sizeof(FIL));
	sens->saveRate = GPS_ERROR_TIME;
	sens->uart = &huart7;
	sens->saveLock = 0;
	sens->buforSize = 0;
	RTC_DateTypeDef date;
	HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
	sprintf(sens->path,"GPS%02d%02d.csv",date.Date,date.Month);
	//HAL_UART_Transmit(sens->uart, commands[1], strlen(commands[1]), HAL_MAX_DELAY); // zmiana baudrate GPSa
	//ReInit_UART1(); // zmiana BaudRate na stm
	HAL_UART_Receive_IT(sens->uart, &(sens->Rx_data), 1); // aktywacja przerwan
	HAL_Delay(100);
	HAL_UART_Transmit(sens->uart, commands[0], strlen(commands[0]), HAL_MAX_DELAY);
	HAL_Delay(100);
	//HAL_UART_Transmit(&huart3, (sens->data), strlen(sens->data), HAL_MAX_DELAY);

	HAL_UART_Transmit(sens->uart, commands[2], strlen(commands[2]), HAL_MAX_DELAY);
	HAL_Delay(100);
	//HAL_UART_Transmit(&huart3, (sens->data), strlen(sens->data), HAL_MAX_DELAY);

	HAL_UART_Transmit(sens->uart, commands[3], strlen(commands[3]), HAL_MAX_DELAY);
	HAL_Delay(100);
	//HAL_UART_Transmit(&huart3, (sens->data), strlen(sens->data), HAL_MAX_DELAY);
}
/* *******ADC SECTION  ********/
extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern ADC_HandleTypeDef hadc3;
void adcInit(ADCSensor* sens,ADC_HandleTypeDef * adc,int channel,FIL* f)
{
	if(f == 0)
	{
		sens->File = (FIL*)malloc(sizeof(FIL));
	}else
	{
		sens->File = f;
	}
	sens->adcChannel = channel;
	sens->adc = adc;
	sens->timeFromLastSuccRead = 0;
}
void damperInit(ADCSensor* sens,SENSORS id,FIL * f){
	switch(id){
	case DAMPERLF:
		adcInit(sens,&hadc3,12,f);
		statusRegister.DamperLF = SENSOR_OK;
		break;
	case DAMPERRF:
		adcInit(sens,&hadc3,13,f);
		statusRegister.DamperRF = SENSOR_OK;
		break;
	default:
		break;
	}
	sens->dataReady = 0;
	RTC_DateTypeDef date;
	HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
	sprintf(sens->path,"DAMP%02d%02d.csv",date.Date,date.Month);
	sens->ID = id;
	sens->adcType = damper;
	sens->timeToNextRead = DAMPER_DATA_RATE;

}
void steeringInit(ADCSensor* sens){

	adcInit(sens,&hadc1,0,0);
	statusRegister.Steering = SENSOR_OK;
	sens->ID = WHEEL;
	RTC_DateTypeDef date;
	HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
	sprintf(sens->path,"WHEEl%02d%02d.csv",date.Date,date.Month);
	sens->adcType = steeringWheel;
	sens->timeToNextRead = STEERING_DATA_RATE;
}
void ADC_SetActiveChannel(ADCSensor* sens)
{
  ADC_ChannelConfTypeDef sConfig = {0};
  sConfig.Channel = sens->adcChannel;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  if (HAL_ADC_ConfigChannel(sens->adc, &sConfig) != HAL_OK)
  {
   Error_Handler();
  }
}
void adcGetData(ADCSensor * sens){
	ADC_SetActiveChannel(sens);
	HAL_ADC_Start(sens->adc);
	HAL_ADC_PollForConversion(sens->adc, HAL_MAX_DELAY);
	sens->data = HAL_ADC_GetValue(sens->adc);
}
/******** ABS SECTION  ********/
void absInit(ABSSensor * sens,SENSORS id,TIM_HandleTypeDef* tim,int channel,FIL *f){
	if(f == 0)
	{
		sens->File = (FIL*)malloc(sizeof(FIL));
	}
	switch(id){
	case ABSLF:
		statusRegister.VSSLF = SENSOR_OK;
		break;

	case ABSRF:
		statusRegister.VSSRF = SENSOR_OK;
		break;
	}
	sens->dataReady = 0;
	sens->ID =id;
	RTC_DateTypeDef date;
	HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
	sprintf(sens->path,"ABS%02d%02d.csv",date.Date,date.Month);
	sens->timer = tim;
	sens->timerChannel = channel;
}

float absCalculate(int data)
{
//kąt 1/12
//Średnica opony: 15"?
//Prędkośc = czas * (średnica*kąt*PI)
	float res = (1/12)*38.1*data*M_PI;
	return res;
}
/* *******GYRO SECTION  ********/

//GYRO FUNCS
void gyroInit(GyroSensor * sens)
{
	int result = MPU9250_Init();
	if(result == 0)
	{
		statusRegister.GYRO = SENSOR_OK;
	}else{
		statusRegister.GYRO = SENSOR_INIT_FAIL;
	}
	sens->File = (FIL*)malloc(sizeof(FIL));

	RTC_DateTypeDef date;
	HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
	sprintf(sens->path,"GYRO%02d%02d.csv",date.Date,date.Month);
	sens->dataReady = 0;
	sens->saveRate = GYRO_DATA_RATE;
	sens->timeToNextRead = GYRO_DATA_RATE;
}
void gyroConvertData(struct gyroData * input, struct gyroDataCalc * output)
{
	output->acc_data_calc[0] = (double) input->acc_data[0] / 16384;
	output->acc_data_calc[1] = (double) input->acc_data[1] / 16384;
	output->acc_data_calc[2] = (double) input->acc_data[2] / 16384;
	output->gyro_data_calc[0] = (double) input->gyro_data[0] * 250 / 32768;
	output->gyro_data_calc[1] = (double) input->gyro_data[1] * 250 / 32768;
	output->gyro_data_calc[2] = (double) input->gyro_data[2] * 250 / 32768;

}

void gyroGetData(GyroSensor * sens)
{
	gyroData imu_9dof_data;
    MPU9250_GetData(imu_9dof_data.acc_data, imu_9dof_data.mag_data, imu_9dof_data.gyro_data);
    gyroConvertData(&imu_9dof_data, &sens->data);
    sens->dataReady = 0;
}


/* *******MLX90640 SECTION  ********/



//MLX FUNCS
int mlxInit(MLXSensor *mlx,SENSORS id, I2C_HandleTypeDef * i2c,FIL * f)
{
		uint16_t eeMLX90640[832];
		if(f == 0)
		{
			mlx->File = (FIL*)malloc(sizeof(FIL));
		}else
		{
			mlx->File = f;
		}
		RTC_DateTypeDef date;
		HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
		sprintf(mlx->path,"MLX%02d%02d.csv",date.Date,date.Month);
		mlx->timeToNextRead = MLX_DATA_RATE;
		mlx->dataReady = 0;
	  	mlx->emissivity = 0.95;
	  	mlx->ID = id;
		MLX90640_SetRefreshRate(MLX90640_ADDR, RefreshRate,mlx->i2c);
	  	MLX90640_SetChessMode(MLX90640_ADDR,mlx->i2c);
	    int status = MLX90640_DumpEE(MLX90640_ADDR, eeMLX90640,mlx->i2c);
	    status = status | MLX90640_ExtractParameters(eeMLX90640, &mlx->mlx90640);
	    if (status != 0){
	    	 if(mlx->ID == MLXLF)
	    	 {
	    	    statusRegister.MLXLF = SENSOR_INIT_FAIL;
	    	 }else if(mlx->ID == MLXRF)
	    	 {
	    	    statusRegister.MLXRF = SENSOR_INIT_FAIL;
	    	 }
	    	 return status;
	   }
	    mlx->i2c = i2c;

	    if(mlx->ID == MLXLF)
	    {
	    	statusRegister.MLXLF = SENSOR_OK;
	    }else if(mlx->ID == MLXRF)
	    {
	    	statusRegister.MLXRF = SENSOR_OK;
	    }


	    return 0;
}

//Copy and calcualte data from sensor memory to stm
int mlxGetData(MLXSensor* mlx){
	uint16_t frame[832];
	int status = MLX90640_GetFrameData(MLX90640_ADDR, frame,mlx->i2c);
	if (status < 0)
	{
		if(DEBUG)
			printf("Error while getting frame 1: %d\n",status);
		return status;
	}

	mlx->vdd = MLX90640_GetVdd(frame, &mlx->mlx90640);
	mlx->ambientTemp = MLX90640_GetTa(frame, &mlx->mlx90640) - TA_SHIFT;

	MLX90640_CalculateTo(frame, &mlx->mlx90640, mlx->emissivity , mlx->ambientTemp, mlx->data);
	status = MLX90640_GetFrameData(MLX90640_ADDR, frame,mlx->i2c);
	if(status < 0)
	{
		if(DEBUG)
			printf("Error while getting frame 2: %d\n",status);
		return status;
	}
	//mlx->dataReady = 1;
	MLX90640_CalculateTo(frame, &mlx->mlx90640, mlx->emissivity , mlx->ambientTemp, mlx->data);
	return 0;
}

//DEBUG printing on default uart
void mlxPrintData(MLXSensor* mlx)
{
	for(int i = 0; i < 768; i++){
		if(i%32 == 0 && i != 0){
			printf("\r\n");
		}
		printf("%2.2f ",mlx->data[i]);
	}
}
