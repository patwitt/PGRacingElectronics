#ifndef INC_DEFINE_CONFIG_H
#define INC_DEFINE_CONFIG_H

/* General configs */
#define CONFIG_RUN_DEBUG                 (FALSE)

/* DBW configs */
#define CONFIG_DBW_ENABLE                (FALSE)
#define CONFIG_DBW_CALIBRATE_TPS_AUTO    (TRUE)
#define CONFIG_DBW_ADJUST_SENS_LIMITS    (FALSE)

/* Clutch config */
#define CONFIG_CLUTCH_ENABLE             (FALSE)

/* ADC configs */
#define CONFIG_ADC_SHOW_MIN_MAX          (TRUE)

/* CAN BUS config */
#define CONFIG_CAN_ENABLE                (FALSE)

/* PID configs */
#define CONFIG_PID_ENABLE_RC_LPF         (FALSE)
#define CONFIG_PID_ENABLE_IIR            (TRUE)
#define CONFIG_PID_INTERPOLATE_CONSTANTS (FALSE)
#define CONFIG_PID_APPLY_BRAKE           (TRUE)

#endif // INC_DEFINE_CONFIG_H
