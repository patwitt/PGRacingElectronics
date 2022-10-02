#include "WS2812_driver.h"

#include "cmsis_os2.h"
#include "math.h"
#include "structs/display_data.h"
#include "structs/telemetry_data.h"
#include "tim.h"

volatile uint16_t leds[RESET_LEN + 24 * LED_NUMBER];

static uint16_t optimalShiftUpRPM[6];
static const float GearRatios[6] =
{ 2.615, 1.857, 1.565, 1.35, 1.238, 1.136 };
// torque from 3000 rpm to 12000
static const uint8_t torque[10] =
{ 30, 40, 65, 70, 60, 58, 58, 55, 45, 40 };

static void updateLedsType_0(int8_t activeLeds);
static void updateLedsType_1(int8_t activeLeds);
static float lerp(float A, float B, float Alpha);

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	HAL_TIM_PWM_Stop_DMA(htim, TIM_CHANNEL_1);
}

void WS2812_Init(void)
{
	for (int i = 0; i < 24 * LED_NUMBER; i++)
		leds[i] = BIT_0_TIME;

	for (int i = 0; i < RESET_LEN; i++)
		leds[i + LED_NUMBER * 24] = 0;

	HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t*) leds,
	RESET_LEN + 24 * LED_NUMBER);
	for (uint8_t i = 1; i <= LED_NUMBER; i++)
	{
		updateLedsType_0(i);
		osDelay(50);
	}
}

void setLed(uint8_t ledNumber, uint8_t red, uint8_t green, uint8_t blue)
{
	const uint8_t lookUpTable[8] =
	{ 1, 2, 4, 8, 16, 32, 64, 128 };
	uint16_t ledId = 24 * ledNumber;
	for (int i = 0; i < 8; i++)
	{
		leds[ledId] = red & lookUpTable[i] ? BIT_1_TIME : BIT_0_TIME;
		leds[ledId + 8] = green & lookUpTable[i] ? BIT_1_TIME : BIT_0_TIME;
		leds[ledId + 16] = blue & lookUpTable[i] ? BIT_1_TIME : BIT_0_TIME;
		ledId++;
	}
}

// turn on leds linearly
void updateLedsType_0(int8_t activeLeds)
{
	for (uint8_t i = 0; i < LED_NUMBER; i++)
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
				setLed(i, 0, 0, displaySetup.blueLedBrightness);
			}
		}
		else
		{
			setLed(i, 0, 0, 0);
		}

	}
	//__DSB();
	HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t*) leds,
	RESET_LEN + 24 * LED_NUMBER);
}

// turn on leds 1-10 linearly, last five turn on together
void updateLedsType_1(int8_t activeLeds)
{
	for (uint8_t i = 0; i < LED_NUMBER; i++)
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
				for (uint8_t j = i; j < LED_NUMBER; j++)
				{
					setLed(i, 0, 0, displaySetup.blueLedBrightness);
				}
			}
		}
		else
		{
			setLed(i, 0, 0, 0);
		}
	}
	//__DSB();
	HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t*) leds,
	RESET_LEN + 24 * LED_NUMBER);
}

void updateLeds(uint16_t rpm, uint8_t mode)
{
	uint16_t minRPM;
	uint16_t maxRPM;
	int8_t activeLeds;
	static uint8_t lastGear = 1;
	switch (mode)
	{
	case 0:
		minRPM = 0;
		maxRPM = 12000;
		activeLeds =
				ceil(
						(float) (rpm - minRPM)
								/ (float) (maxRPM - minRPM)* LED_NUMBER);
		break;
	case 1:
	case 2:
#ifdef USE_OPTIMAL_SHIFT_RPM
		if (telemetryData.gear <= 6 && telemetryData.gear >= 1)
		{
			lastGear = telemetryData.gear;
		}

		minRPM = optimalShiftUpRPM[lastGear - 1] - 2000;
		maxRPM = optimalShiftUpRPM[lastGear - 1];
#else
		minRPM = 9500;
		maxRPM = 11500;
#endif
		uint8_t ledNumber = mode == 1 ? LED_NUMBER : LED_NUMBER - 4;
		activeLeds = ceil(
				(float) (rpm - minRPM) / (float) (maxRPM - minRPM) * ledNumber);

		break;

	}

	switch (mode)
	{
	case 0:
	case 1:
		updateLedsType_0(activeLeds);
		break;
	case 2:
		updateLedsType_1(activeLeds);
		break;
	}
}

float lerp(float A, float B, float Alpha)
{
	return A + Alpha * (B - A);
}

void ComputeOptimalPoints()
{
// Calculation based on https://glennmessersmith.com/shiftpt.html
	const uint16_t minRPM = 3000;
	const uint16_t maxRPM = 12000;
	uint16_t UpRPM; // DownRPM;
#ifdef MY_OPTIMAL_SHIFT_IMPLEMENTATION
	for (int i = 0; i < GEAR_NUMBER- 1; i++)
	{
		float MinTorqueLoss = MAX_ENGINE_TORQUE *  GearRatios[i];
		float CurrentGearRatio = GearRatios[i];
		float NextGearRatio = GearRatios[i + 1];
		UpRPM = minRPM;
		for (int j = minRPM; j < maxRPM; j += 50)
		{
			float EngineTorque = lerp(torque[(j-3000)/1000],torque[(j-2000)/1000], (j%1000)/1000.0f);
			// calculate current torque on gearbox output
			float CurrentGearTorque = EngineTorque * CurrentGearRatio;
			// calculate engine RPM on next gear
			uint16_t NextRPM = NextGearRatio * j / CurrentGearRatio;
			if (NextRPM < minRPM)
			{
				continue;
			}
			float NextGearEngineTorque = lerp(torque[(NextRPM-3000)/1000],torque[(NextRPM-2000)/1000], (NextRPM%1000)/1000.0f);
			// calculate torque on gearbox outpur on next gear
			float NextGearTorque = NextGearEngineTorque * NextGearRatio;

			if (CurrentGearTorque - NextGearTorque < MinTorqueLoss && CurrentGearTorque - NextGearTorque >= -0.01* MAX_ENGINE_TORQUE *  GearRatios[i])
			{
				UpRPM = j;
				//DownRPM = NextRPM;
				MinTorqueLoss = CurrentGearTorque - NextGearTorque;
			}
		}
		optimalShiftUpRPM[i]=UpRPM;
		//OptimalGearDownRPM.Add(DownRPM);
	}
#else
	for (int i = GEAR_NUMBER - 2; i >= 0; i--)
	{
		float MinTorqueLoss = MAX_ENGINE_TORQUE * GearRatios[i];
		float CurrentGearRatio = GearRatios[i];
		float NextGearRatio = GearRatios[i + 1];
		UpRPM = maxRPM;
		for (int j = maxRPM; j > minRPM; j -= 50)
		{
			float EngineTorque = lerp(torque[(j - 3000) / 1000],
					torque[(j - 2000) / 1000], (j % 1000) / 1000.0f);
			// calculate current torque on gearbox output
			float CurrentGearTorque = EngineTorque * CurrentGearRatio;
			// calculate engine RPM on next gear
			uint16_t NextRPM = NextGearRatio * j / CurrentGearRatio;
			if (NextRPM < minRPM)
			{
				continue;
			}
			float NextGearEngineTorque = lerp(torque[(NextRPM - 3000) / 1000],
					torque[(NextRPM - 2000) / 1000],
					(NextRPM % 1000) / 1000.0f);
			// calculate torque on gearbox outpur on next gear
			float NextGearTorque = NextGearEngineTorque * NextGearRatio;
			float deltaTorque = CurrentGearTorque - NextGearTorque;
			float absDeltaTorque =
					deltaTorque < 0.0f ? -deltaTorque : deltaTorque;
			if (absDeltaTorque < MinTorqueLoss)
			{
				UpRPM = j;
				MinTorqueLoss = absDeltaTorque;
			}
			if (deltaTorque > 0.0f)
			{
				break;
			}
		}
		optimalShiftUpRPM[i] = UpRPM;
		//OptimalGearDownRPM.Add(DownRPM);
	}
#endif
	optimalShiftUpRPM[5] = 12000;
}

