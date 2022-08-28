/*
 * InjectorsCut.c
 *
 *  Created on: 12.07.2022
 *      Author: Patryk Wittbrodt
 */
#include "InjectorsCut.h"
#if CONFIG_ENABLE_INJECTORS_CUT
#include "GearWatchdog.h"
#include "stm32f4xx_hal.h"
#include "main.h"
#include "CAN.h"

/* ---------------------------- */
/*         Local data           */
/* ---------------------------- */

#define CAN_BYTE_INJECTORS_TRIGGER (0xFFU)
#define CAN_BYTE_INJECTORS_RESTORE (0x00U)

//! Injectors Cut handler struct
typedef struct {
	GearWatchdogType *const watchdog;
	GPIO_TypeDef *const gpioPort;
	const uint16_t gpioPin;
	const CAN_MsgDataBytes canByte;
	const CAN_TxMsgEnum canMsgId;
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
	.gpioPin  = GEAR_CUT_Pin,
	.canByte = CAN_DATA_BYTE_5,
	.canMsgId = CAN_TX_MSG_INJ_CUT
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
#if INJECTORS_CUT_OG
	HAL_GPIO_WritePin(injectorsCut.gpioPort, injectorsCut.gpioPin, GPIO_PIN_SET);
#elif INJECTORS_CUT_ECU
	CAN_TxUpdateAllBytes(injectorsCut.canMsgId, CAN_BYTE_INJECTORS_RESTORE);
	CAN_TxScheduleMsg(injectorsCut.canMsgId);
#endif
}

/**
 * @brief Disable the injectors.
 *
 * Set the pin to low.
 */
static inline void InjectorsCut_DisableInjectors(void)
{
#if INJECTORS_CUT_OG
	HAL_GPIO_WritePin(injectorsCut.gpioPort, injectorsCut.gpioPin, GPIO_PIN_RESET);
#elif INJECTORS_CUT_ECU
	CAN_TxUpdateAllBytes(injectorsCut.canMsgId, CAN_BYTE_INJECTORS_TRIGGER);
	CAN_TxScheduleMsg(injectorsCut.canMsgId);
#endif
}

/**
 * @brief Watchdog elapsed function.
 *
 * If the watchdog timer has elapsed, disable the injectors cut and enable the injectors again.
 * 
 * The function is called by the watchdog timer interrupt
 */
static void InjectorsCutWatchdogElapsedTrigger(void)
{
	/* Watchdog time elapsed */
	/* Disable Injectors Cut, enable injectors again */
	InjectorsCut_EnableInjectors();
}
/* ---------------------------- */
/*       Global functions       */
/* ---------------------------- */

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
	/* Start Watchdog */
	GearWatchdog_Start(injectorsCut.watchdog);
	/* Trigger injectors cut, disable injectors */
	InjectorsCut_DisableInjectors();
}

/**
 * @brief Finish the Injectors Cut.
 * 
 * If the injectors cut is enabled, feed the watchdog and enable the injectors.
 */
void InjectorsCut_Finish(void)
{
	/* Feed watchdog, notice of successful shift on time */
	GearWatchdog_Feed(injectorsCut.watchdog);

	/* Disable Injectors Cut, enable injectors again */
	InjectorsCut_EnableInjectors();
}
#else
void InjectorsCut_Trigger(void) {}
void InjectorsCut_Finish(void) {}
ErrorEnum InjectorsCut_Init(void) { return ERROR_OK; }
#endif
