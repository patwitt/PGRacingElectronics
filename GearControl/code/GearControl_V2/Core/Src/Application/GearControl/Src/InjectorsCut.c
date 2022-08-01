/*
 * InjectorsCut.c
 *
 *  Created on: 12.07.2022
 *      Author: Patryk Wittbrodt
 */
#include "InjectorsCut.h"
#include "GearWatchdog.h"
#include "stm32f4xx_hal.h"
#include "main.h"

/* ---------------------------- */
/*         Local data           */
/* ---------------------------- */

static void InjectorsCutWatchdogElapsedTrigger(void);

static GearWatchdogType injectorsCutWdg = {
		.elapsedTrigger = InjectorsCutWatchdogElapsedTrigger,
		.status = GEAR_WATCHDOG_STATUS_INACTIVE,
		.timeoutMs = 100U
};

typedef struct {
	GearWatchdogType *const watchdog;
	GPIO_TypeDef *const gpioPort;
	const uint16_t gpioPin;
} InjectorsCutHandler;

static InjectorsCutHandler injectorsHandler = {.watchdog = &injectorsCutWdg,
		                                       .gpioPort = GEAR_CUT_GPIO_Port,
											   .gpioPin  = GEAR_CUT_Pin
};

/* ---------------------------- */
/* Local function declarations  */
/* ---------------------------- */
static inline void InjectorsCut_EnableInjectors(void);
static inline void InjectorsCut_DisableInjectors(void);

/* ---------------------------- */
/*        Local functions       */
/* ---------------------------- */
static inline void InjectorsCut_EnableInjectors(void)
{
	HAL_GPIO_WritePin(injectorsHandler.gpioPort, injectorsHandler.gpioPin, GPIO_PIN_SET);
}

static inline void InjectorsCut_DisableInjectors(void)
{
	HAL_GPIO_WritePin(injectorsHandler.gpioPort, injectorsHandler.gpioPin, GPIO_PIN_RESET);
}

/* ---------------------------- */
/*       Global functions       */
/* ---------------------------- */
void InjectorsCutWatchdogElapsedTrigger(void)
{
	/* Watchdog time elapsed */
	/* Disable Injectors Cut, enable injectors again */
	InjectorsCut_EnableInjectors();
}

ErrorEnum InjectorsCut_Init(void)
{
	ErrorEnum err = ERROR_OK;

	/* Initialize Injectors Cut Watchdog */
	err = GearWatchdog_Init(injectorsHandler.watchdog);

	return err;
}

void InjectorsCut_Trigger(void)
{
	/* Start Watchdog */
	GearWatchdog_Start(injectorsHandler.watchdog);

	/* Trigger injectors cut, disable injectors */
	InjectorsCut_DisableInjectors();
}

void InjectorsCut_Finish(void)
{
	/* Feed watchdog, notice of successful shift on time */
	GearWatchdog_Feed(injectorsHandler.watchdog);

	/* Disable Injectors Cut, enable injectors again */
	InjectorsCut_EnableInjectors();
}
