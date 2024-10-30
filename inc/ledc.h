#ifndef __LEDC_H__
#define __LEDC_H__

#include "led_ctrl.h"

#ifdef __cplusplus
extern "C"
{
#endif

LEDIndicateHandle init_led_indicate(LED_t handle);

void led_indicate_set(LEDIndicateHandle p, LED_Priority priority, LED_On_Status on_status, uint16_t R, uint16_t G, uint16_t B);
void led_indicate_set_blink(LEDIndicateHandle p, LED_Priority priority, uint16_t R, uint16_t G, uint16_t B, uint32_t onTimeMS, uint32_t offTimeMS);
void led_indicate_stop(LEDIndicateHandle p);
void led_indicate_start(LEDIndicateHandle p);
void led_indicate_set_done(LEDIndicateHandle p, LED_Priority priority);
void led_indicate_set_done_run(LEDIndicateHandle p);
void led_indicate_set_count_blink(LEDIndicateHandle p, LED_Priority priority, LED_On_Status on_status, uint16_t R, uint16_t G, uint16_t B,
                                    uint32_t onTimeMS, uint32_t offTimeMS, uint16_t count);
void led_indicate_set_mask(LEDIndicateHandle p, LED_Priority priority);
void led_indicate_cancel_mask(LEDIndicateHandle p, LED_Priority priority);

#ifdef __cplusplus
}
#endif

#endif
