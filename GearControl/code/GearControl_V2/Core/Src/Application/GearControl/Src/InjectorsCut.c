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

//! Injectors Cut handler struct
typedef struct {
	GearWatchdogType *const watchdog;
	GPIO_TypeDef *const gpioPort;
	const uint16_t gpioPin;
} InjectorsCutHandler;

//! This must be before injectorsCutWdg declaration
static void InjectorsCutWatchdogElapsedTrigger(void);

//! Injectors Cut watchdog
static GearWatchdogType injectorsCutWdg = {
		.elapsedTrigger = InjectorsCutWatchdogElapsedTrigger,
		.status = GEAR_WATCHDOG_STATUS_INACTIVE,
		.timeoutMs = INJECTORS_CUT_WATCHDOG_TIMEOUT_MS
};

//! Injectors Cut handler
static InjectorsCutHandler injectorsCut = {
	.watchdog = &injectorsCutWdg,
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
/**
 * @brief Enable the injectors.
 *
 * Set the pin to high.
 */
static inline void InjectorsCut_EnableInjectors(void)
{
	HAL_GPIO_WritePin(injectorsCut.gpioPort, injectorsCut.gpioPin, GPIO_PIN_SET);
}

/**
 * @brief Disable the injectors.
 *
 * Set the pin to low.
 */
static inline void InjectorsCut_DisableInjectors(void)
{
	HAL_GPIO_WritePin(injectorsCut.gpioPort, injectorsCut.gpioPin, GPIO_PIN_RESET);
}

/* ---------------------------- */
/*       Global functions       */
/* ---------------------------- */
/**
 * @brief Watchdog elapsed function.
 *
 * If the watchdog timer has elapsed, disable the injectors cut and enable the injectors again.
 * 
 * The function is called by the watchdog timer interrupt
 */
void InjectorsCutWatchdogElapsedTrigger(void)
{
	/* Watchdog time elapsed */
	/* Disable Injectors Cut, enable injectors again */
	InjectorsCut_EnableInjectors();
}

/**
 * @brief Initialization of the Injectors Cut module.
 * 
 * @return an error code.
 */
ErrorEnum InjectorsCut_Init(void)
{
	ErrorEnum err = ERROR_OK;

	/* Initialize Injectors Cut Watchdog */
	err = GearWatchdog_Init(injectorsCut.watchdog);

	return err;
}

/**
 * @brief Trigger the Injectors Cut.
 * 
 * If the injectors cut feature is enabled, start the watchdog and disable the injectors.
 */
void InjectorsCut_Trigger(void)
{
#if CONFIG_ENABLE_INJECTORS_CUT
	/* Start Watchdog */
	GearWatchdog_Start(injectorsCut.watchdog);

	/* Trigger injectors cut, disable injectors */
	InjectorsCut_DisableInjectors();
#endif
}

/**
 * @brief Finish the Injectors Cut.
 * 
 * If the injectors cut is enabled, feed the watchdog and enable the injectors.
 */
void InjectorsCut_Finish(void)
{
#if CONFIG_ENABLE_INJECTORS_CUT
	/* Feed watchdog, notice of successful shift on time */
	GearWatchdog_Feed(injectorsCut.watchdog);

	/* Disable Injectors Cut, enable injectors again */
	InjectorsCut_EnableInjectors();
#endif
}
