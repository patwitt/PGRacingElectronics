#ifndef INC_DEFINE_CONFIG_H
#define INC_DEFINE_CONFIG_H

/* General configs */
#define CONFIG_RUN_DEBUG                 (FALSE) //!< Run build with additional debug information

/* !ENABLE configs! */
#define CONFIG_ENABLE_DBW                (TRUE)  //!< Enable Drive-By-Wire
#define CONFIG_ENABLE_CLUTCH             (TRUE)  //!< Enable Clutch operations
#define CONFIG_ENABLE_CAN                (FALSE) //!< Enable CAN Bus
#define CONFIG_ENABLE_INJECTORS_CUT      (TRUE)  //!< Enable Injectors Cut on gearshifts
#define CONFIG_ENABLE_CLUTCH_SLIP        (TRUE)  //!< Enable Clutch Slip on gearshifts

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
