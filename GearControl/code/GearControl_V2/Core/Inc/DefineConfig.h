#ifndef INC_DEFINE_CONFIG_H
#define INC_DEFINE_CONFIG_H

#include "Types.h"

#define FEATURE_ENABLED (TRUE)
#define FEATURE_DISABLE (FALSE)

/* General configs */
#define CONFIG_RUN_DEBUG                 (FALSE) //!< Run build with additional debug information

/* !ENABLE/DISABLE feature configs! */
#define CONFIG_ENABLE_SCHEDULER_WATCHDOG (FEATURE_DISABLE) //!< Enable Scheduler Watchdog
#define CONFIG_ENABLE_DBW                (FEATURE_ENABLED) //!< Enable Drive-By-Wire
#define CONFIG_ENABLE_CLUTCH             (FEATURE_ENABLED) //!< Enable Clutch operations
#define CONFIG_ENABLE_CAN                (FEATURE_DISABLE) //!< Enable CAN Bus
/* !Gear shifting ENABLE/DISABLE feature configs! */
#define CONFIG_ENABLE_INJECTORS_CUT      (FEATURE_ENABLED) //!< Enable Injectors Cut on gearshifts
#define CONFIG_ENABLE_CLUTCH_SLIP        (FEATURE_ENABLED) //!< Enable Clutch Slip on gearshifts
#define CONFIG_ENABLE_REV_MATCH          (FEATURE_ENABLED) //!< Enable rev matching

/* GearControl configs */
#define REVMATCH_FROM_ENGINE_RPM (TRUE)                      //!< Revmatching from Engine RPM's
#define REVMATCH_FROM_WHEEL_RPM  (!REVMATCH_FROM_ENGINE_RPM) //!< Revmatching from Wheel RPM's

/* DBW configs */
#define CONFIG_DBW_CALIBRATE_TPS_AUTO    (TRUE)  //!< Automatically calibrate TPS thresholds
#define CONFIG_DBW_ADJUST_SENS_LIMITS    (FALSE) //!< Automatically adjust TPS and APPS sensor limits

/* ADC configs */
#define CONFIG_ADC_SHOW_MIN_MAX          (TRUE) //!< Add min/max on ADC readings

/* PID configs */
#define CONFIG_PID_ENABLE_RC_LPF         (FALSE) //!< Enable RC Low-Pass Filter for PID inputs
#define CONFIG_PID_ENABLE_IIR            (TRUE)  //!< Enable IIR Filter for PID Inputs
#define CONFIG_PID_INTERPOLATE_CONSTANTS (FALSE) //!< Interpolate PID constants over 1D Table
#define CONFIG_PID_APPLY_BRAKE           (TRUE)  //!< Apply PID Brake. Should be enabled for good performance

#endif // INC_DEFINE_CONFIG_H
