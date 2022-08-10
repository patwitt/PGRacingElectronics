/*
 * PID_InterpolationTables.h
 *
 *  Created on: 06.06.2022
 *      Author: Patryk Wittbrodt
 */

#ifndef SRC_APPLICATION_PID_INTERP_TABLES_H_
#define SRC_APPLICATION_PID_INTERP_TABLES_H_

#include "Types.h"
#include "Utils.h"

/* Kp, Ki, Kd gains look-up table array sizes */
#define KP_LUT_CNT (8U)
#define KI_LUT_CNT (8U)
#define KD_LUT_CNT (8U)

typedef enum {
	PID_INTEPROLATE_LPF_TABLE,
	PID_INTEPROLATE_WORKING_TABLE,
	PID_INTERPOLATE_TABLE_COUNT
} PID_InterpolationTableEnum;

/* --------------------------- TABLE LPF --------------------------- */
/* Kp gains look-up table */
static const float Kp_X_err_1[KP_LUT_CNT] = {5.0f,  10.0f, 40.0f, 130.0f, 570.0f, 710.4f, 860.7f, 1000.0f};
static const float Kp_Y_1[KP_LUT_CNT]     = {6.0f, 6.0f,  6.0f,   5.0f,   5.0f,   5.0f,   4.5f,   4.5f};
static const table_1d table1d_Kp_1 = {.x_values = &Kp_X_err_1[0U], .y_values = &Kp_Y_1[0U], .x_length = KP_LUT_CNT};

/* Ki gains look-up table */
static const float Ki_X_err_1[KI_LUT_CNT] = {4.0f,  5.0f, 20.0f, 30.0f, 40.0f, 50.0f, 60.0f, 100.0f};
static const float Ki_Y_1[KI_LUT_CNT]     = {2.9f, 2.6f,  2.3f,  2.3f,  3.3f,  3.3f,  5.0f,  5.0f};
static const table_1d table1d_Ki_1 = {.x_values = &Ki_X_err_1[0U], .y_values = &Ki_Y_1[0U], .x_length = KI_LUT_CNT};

/* Kd gains look-up table */
static const float Kd_X_err_1[KD_LUT_CNT] = {0.0f,  180.0f, 185.0f, 250.0f, 260.0f, 270.0f, 280.0f, 290.0f};
static const float Kd_Y_1[KD_LUT_CNT]     = {0.09f, 0.09f,  0.09f,   0.09f,  0.09f,   0.09f,   0.09f,   0.09f};
static const table_1d table1d_Kd_1 = {.x_values = &Kd_X_err_1[0U], .y_values = &Kd_Y_1[0U], .x_length = KD_LUT_CNT};

/* --------------------------- TABLE WORKING --------------------------- */
/* Kp gains look-up table */
static const float Kp_X_err_2[KP_LUT_CNT] = {5.0f,  10.0f, 40.0f, 130.0f, 570.0f, 710.4f, 860.7f, 1000.0f};
static const float Kp_Y_2[KP_LUT_CNT]     = {7.0f, 7.0f,  7.0f,   6.5f,   6.0f,   5.0f,   4.5f,   4.5f};
static const table_1d table1d_Kp_2 = {.x_values = &Kp_X_err_2[0U], .y_values = &Kp_Y_2[0U], .x_length = KP_LUT_CNT};

/* Ki gains look-up table */
static const float Ki_X_err_2[KI_LUT_CNT] = {4.0f,  5.0f, 20.0f, 30.0f, 40.0f, 50.0f, 60.0f, 100.0f};
static const float Ki_Y_2[KI_LUT_CNT]     = {2.9f, 2.6f,  2.3f,  2.3f,  3.3f,  3.3f,  5.0f,  5.0f};
static const table_1d table1d_Ki_2 = {.x_values = &Ki_X_err_2[0U], .y_values = &Ki_Y_2[0U], .x_length = KI_LUT_CNT};

/* Kd gains look-up table */
static const float Kd_X_err_2[KD_LUT_CNT] = {0.0f,  180.0f, 185.0f, 250.0f, 260.0f, 270.0f, 280.0f, 290.0f};
static const float Kd_Y_2[KD_LUT_CNT]     = {0.29f, 0.29f,  0.29f,   0.29f,  0.29f,   0.29f,   0.29f,   0.29f};
static const table_1d table1d_Kd_2 = {.x_values = &Kd_X_err_2[0U], .y_values = &Kd_Y_2[0U], .x_length = KD_LUT_CNT};

typedef struct {
	const table_1d *const table1d_Kp;
	const table_1d *const table1d_Ki;
	const table_1d *const table1d_Kd;
} PID_InterpolationTable;

static const PID_InterpolationTable PID_InterpolationTables[PID_INTERPOLATE_TABLE_COUNT] = {
		[PID_INTEPROLATE_LPF_TABLE] = {
			.table1d_Kp = &table1d_Kp_1,
			.table1d_Ki = &table1d_Ki_1,
			.table1d_Kd = &table1d_Kd_1,
		},
		[PID_INTEPROLATE_WORKING_TABLE] = {
			.table1d_Kp = &table1d_Kp_2,
			.table1d_Ki = &table1d_Ki_2,
			.table1d_Kd = &table1d_Kd_2,
		}
};

#endif /* SRC_APPLICATION_PID_INTERP_TABLES_H_ */
