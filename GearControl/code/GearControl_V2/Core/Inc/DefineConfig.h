#ifndef INC_DEFINE_CONFIG_H
#define INC_DEFINE_CONFIG_H

#define CLAMP_MAX(val, max) ((val > max) ? max : val)
#define CLAMP_MIN(val, min) ((val < min) ? min : val)

#if 1
#define CALIBRATE_TPS_AUTO
#endif

#define SHOW_MIN_MAX
#define LED_ENABLED

#endif // INC_DEFINE_CONFIG_H
