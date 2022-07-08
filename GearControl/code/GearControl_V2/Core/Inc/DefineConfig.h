#ifndef INC_DEFINE_CONFIG_H
#define INC_DEFINE_CONFIG_H

/* DBW configs */
#define CONFIG_DBW_CALIBRATE_TPS_AUTO    (TRUE)
#define CONFIG_DBW_ADJUST_SENS_LIMITS    (FALSE)

/* ADC configs */
#define CONFIG_ADC_SHOW_MIN_MAX          (TRUE)

/* CAN BUS config */
#define CAN_BUS_ENABLE                   (FALSE)

/* PID configs */
#define CONFIG_PID_ENABLE_RC_LPF         (FALSE)
#define CONFIG_PID_ENABLE_IIR            (TRUE)
#define CONFIG_PID_INTERPOLATE_CONSTANTS (FALSE)
#define CONFIG_PID_APPLY_BRAKE           (TRUE)

#endif // INC_DEFINE_CONFIG_H
