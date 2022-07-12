#ifndef INC_TYPES_H
#define INC_TYPES_H

#include <stdint.h>

/**
 * @file Types.h
 * @brief Definition of Types compatible with STM32 MCU
 */

/**
 * IEE 754 single precision floating point
 */
typedef float float32_t;
typedef float float32;

/**
 * Unsigned 8 bit integer
 */
typedef unsigned char uint8_t;
typedef unsigned char uint8;

/**
 * Unsigned 16 bit integer
 */
typedef unsigned short uint16_t;
typedef unsigned short uint16;

#define     __IO    volatile

#define FALSE (0U)
#define TRUE (1U)


/**
 * Unsigned 32 bit integer
 */
typedef uint32_t uint32;

/**
 * 8 bit (unsigned) character to be used in strings;
 * only permitted operations are assignment, equality and inequality
 */
typedef unsigned char char_t;

/**
 * Boolean 8 bit variable; should only take on values of true/false
 */
typedef _Bool bool_t;
typedef bool_t boolean;

/**
 * Constant to express positive boolean state
 */
#define true (1==1)

/**
 * Constant to express negative boolean state
 */
#define false (1==0)

/**
 * Null pointer definition
 */
#define NULL ((void *)0)

#ifndef VCAST_TEST_BUILD

/**
 * Redefinition of the "const" identifier.
 * It's preferred to use this over the naive "const" because it can be changed during test builds
 * to achieve alternative scenarios.
 */
#define CONST const

/**
 * Macro to invoke the inline assembler
 */
#define ASM(X) __asm X;

#else
#define CONST
#define ASM(X)
#endif

typedef enum {
	ERROR_OK,
	ERROR_NOK,
	ERROR_NULL,
	ERROR_OOR,
	ERROR_HAL,
	ERROR_DBW_TPS_INIT,
	ERROR_DBW_APPS_INIT,
	ERROR_DBW_DC_MOTOR_INIT,
	ERROR_DBW_TPS_CALIBRATION,
	ERROR_DBW_APPS_CALIBRATION,
	ERROR_DBW_TPS_PLAUSIBILITY,
	ERROR_DBW_APPS_PLAUSIBILITY,
	ERROR_SW_TIMER_MAX_EXCEEDED,
	ERROR_GEAR_WATCHDOG_INIT,
	ERROR_SERVO_INIT,
	ERROR_CAN_INVALID_STATUS,
	ERROR_COUNT
} ErrorEnum;

#endif // INC_TYPES_H
