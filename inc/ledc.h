#ifndef __LEDC_H__
#define __LEDC_H__

#include "led_ctrl.h"

#ifdef __cplusplus
extern "C" {
#endif

LEDIndicateHandle init_led_indicate(LEDIndicateCallback callback);

void led_indicate_set(LEDIndicateHandle p, LEDPriority priority,
                      LEDOnStatus on_status, uint16_t R, uint16_t G,
                      uint16_t B);
void led_indicate_set_blink(LEDIndicateHandle p, LEDPriority priority,
                            uint16_t R, uint16_t G, uint16_t B,
                            uint32_t onTimeMS, uint32_t offTimeMS);
void led_indicate_stop(LEDIndicateHandle p);
void led_indicate_start(LEDIndicateHandle p);
void led_indicate_set_done(LEDIndicateHandle p, LEDPriority priority);
void led_indicate_set_done_run(LEDIndicateHandle p);
void led_indicate_set_count_blink(LEDIndicateHandle p, LEDPriority priority,
                                  uint16_t R, uint16_t G, uint16_t B,
                                  uint32_t onTimeMS, uint32_t offTimeMS,
                                  uint16_t count);
void led_indicate_set_mask(LEDIndicateHandle p, LEDPriority priority);
void led_indicate_cancel_mask(LEDIndicateHandle p, LEDPriority priority);

#ifdef __cplusplus
}
#endif

#endif
