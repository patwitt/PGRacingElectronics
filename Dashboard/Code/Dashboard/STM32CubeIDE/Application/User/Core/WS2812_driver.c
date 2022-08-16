#include "WS2812_driver.h"
#include "tim.h"
#include "math.h"
#include "display_setup.h"

#define GEAR_NUMBER 6

#define MAX_ENGINE_TORQUE 70

#define MY_OPTIMAL_SHIFT_IMPLEMENTATION
 displaySetup_t displaySetup = { 70, 2, 110, 110, 110 };

 uint16_t OptimalShiftUpRPM[6];
 static const float GearRatios[6]= {2.615, 1.857, 1.565, 1.35, 	1.238, 1.136};
 // torque from 3000 rpm to 12000
 static const uint8_t torque[10] = {30,40,65,70,60,58,58,55,45,40};


//extern displaySetup_t displaySetup;
//extern TIM_HandleTypeDef htim3;
//extern DMA_HandleTypeDef hdma_tim3_ch1_trig;

#define BIT_0_TIME		43
#define BIT_1_TIME		86

#define RESET_LEN		40
#define LED_N			15

volatile uint16_t leds[RESET_LEN + 24 * LED_N];

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	HAL_TIM_PWM_Stop_DMA(htim, TIM_CHANNEL_1);
}

void WS2812_Init(void)
{
	for (int i = 0; i < 24 * LED_N; i++)
		leds[i] = BIT_0_TIME;

	for (int i = 0; i < RESET_LEN; i++)
		leds[i + LED_N * 24] = 0;

	HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t*) leds,	RESET_LEN + 24 * LED_N);
}

void setLed(uint8_t ledNumber, uint8_t red, uint8_t green, uint8_t blue)
{
	const uint8_t lookUpTable[8] = {1,2,4,8,16,32,64,128};
	uint16_t ledId = 24 * ledNumber;
	for (int i = 0; i < 8; i++)
	{
		leds[ledId] = red & lookUpTable[i] ? BIT_1_TIME : BIT_0_TIME;
		leds[ledId+8] = green & lookUpTable[i] ? BIT_1_TIME : BIT_0_TIME;
		leds[ledId+16] = blue & lookUpTable[i] ? BIT_1_TIME : BIT_0_TIME;
		ledId++;
	}
}

void updateLeds(int rpm, int mode)
{
	uint16_t minRPM = 8000;
	uint16_t maxRPM = 11500;
	if(mode == 1)
	{
		minRPM = 0;
		maxRPM = 12000;
	}
	else
	{
		minRPM = 8500;
		maxRPM = 11500;
	}

	int8_t activeLeds = ceil((float)(rpm - minRPM) / (float)(maxRPM - minRPM) * LED_N);
	for (int i = 0; i < LED_N; i++)
	{
		if (i < activeLeds)
		{
			if (i < 5)
			{
				setLed(i, displaySetup.redLedBrightness, 0, 0);
			}
			else if (i < 10)
			{
				setLed(i, 0, displaySetup.greenLedBrightness, 0);
			}
			else
			{
				if (mode == 3)
				{
					for (int j = i; j < LED_N; j++)
					{
						setLed(j, 0, 0, displaySetup.blueLedBrightness);
					}
					break;
				}
				else
				{
					setLed(i,0,0,displaySetup.blueLedBrightness);
				}
			}
		}
		else
		{
			setLed(i,0,0,0);
		}
	}

	HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t*)leds, RESET_LEN + 24 * LED_N);
}

float Lerp(float A, float B, float Alpha)
{
	return A + Alpha * (B - A);
}
void ComputeOptimalPoints()
{
	// Calculation based on https://glennmessersmith.com/shiftpt.html
	const uint16_t MinRPM = 3000;
	const uint16_t MaxRPM = 12000;
	uint16_t UpRPM;// DownRPM;
#ifdef MY_OPTIMAL_SHIFT_IMPLEMENTATION
	for (int i = 0; i < GEAR_NUMBER- 1; i++)
	{
		float MinTorqueLoss = MAX_ENGINE_TORQUE *  GearRatios[i];
		float CurrentGearRatio = GearRatios[i];
		float NextGearRatio = GearRatios[i + 1];
		UpRPM = MinRPM;
		for (int j = MinRPM; j < MaxRPM; j += 50)
		{
			float EngineTorque = Lerp(torque[(j-3000)/1000],torque[(j-2000)/1000], (j%1000)/1000.0f);
			// calculate current torque on gearbox output
			float CurrentGearTorque = EngineTorque * CurrentGearRatio;
			// calculate engine RPM on next gear
			uint16_t NextRPM = NextGearRatio * j / CurrentGearRatio;
			if (NextRPM < MinRPM)
			{
				continue;
			}
			float NextGearEngineTorque = Lerp(torque[(NextRPM-3000)/1000],torque[(NextRPM-2000)/1000], (NextRPM%1000)/1000.0f);
			// calculate torque on gearbox outpur on next gear
			float NextGearTorque = NextGearEngineTorque * NextGearRatio;

			if (CurrentGearTorque - NextGearTorque < MinTorqueLoss && CurrentGearTorque - NextGearTorque >= -0.01* MAX_ENGINE_TORQUE *  GearRatios[i])
			{
				UpRPM = j;
				//DownRPM = NextRPM;
				MinTorqueLoss = CurrentGearTorque - NextGearTorque;
			}
		}
		OptimalShiftUpRPM[i]=UpRPM;
		//OptimalGearDownRPM.Add(DownRPM);
	}
#else
	for (int i = GEAR_NUMBER- 2; i >=0 ; i--)
		{
			float MinTorqueLoss = MAX_ENGINE_TORQUE *  GearRatios[i];
			float CurrentGearRatio = GearRatios[i];
			float NextGearRatio = GearRatios[i + 1];
			UpRPM = MaxRPM;
			for (int j = MaxRPM; j > MinRPM; j -= 50)
			{
				float EngineTorque = Lerp(torque[(j-3000)/1000],torque[(j-2000)/1000], (j%1000)/1000.0f);
				// calculate current torque on gearbox output
				float CurrentGearTorque = EngineTorque * CurrentGearRatio;
				// calculate engine RPM on next gear
				uint16_t NextRPM = NextGearRatio * j / CurrentGearRatio;
				if (NextRPM < MinRPM)
				{
					continue;
				}
				float NextGearEngineTorque = Lerp(torque[(NextRPM-3000)/1000],torque[(NextRPM-2000)/1000], (NextRPM%1000)/1000.0f);
				// calculate torque on gearbox outpur on next gear
				float NextGearTorque = NextGearEngineTorque * NextGearRatio;
				float deltaTorque = CurrentGearTorque - NextGearTorque;
				float absDeltaTorque = deltaTorque <0.0f ? -deltaTorque: deltaTorque;
				if(absDeltaTorque<MinTorqueLoss)
				{
					UpRPM = j;
					MinTorqueLoss =absDeltaTorque;
				}
				if (deltaTorque >0.0f)
				{
					break;
				}
			}
			OptimalShiftUpRPM[i]=UpRPM;
			//OptimalGearDownRPM.Add(DownRPM);
		}
#endif
}

