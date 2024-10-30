#include "ledc.h"

#include "led_ctrl.h"
#ifdef __cplusplus
extern "C" {
#endif

LEDIndicateHandle init_led_indicate(LED_t handle)
{
    LED_GPIO_INIT();
    return new LEDIndicate{handle};
}

void DeinitLEDIndicate(LEDIndicateHandle p)
{
    if (p != NULL)
        delete static_cast<LEDIndicate *>(p);
}

void led_indicate_set(LEDIndicateHandle p,
                    LED_Priority      priority,
                    LED_On_Status     on_status,
                    uint16_t          R,
                    uint16_t          G,
                    uint16_t          B)
{
    if (p != NULL)
        return static_cast<LEDIndicate *>(p)->Set(priority, on_status, R, G, B);
}

void led_indicate_set_blink(LEDIndicateHandle p,
                            LED_Priority      priority,
                            uint16_t          R,
                            uint16_t          G,
                            uint16_t          B,
                            uint32_t          onTimeMS,
                            uint32_t          offTimeMS)
{
    if (p != NULL)
        return static_cast<LEDIndicate *>(p)->Set(priority, LED_Blink, R, G, B, onTimeMS, offTimeMS);
}

void led_indicate_set_count_blink(LEDIndicateHandle p,
                                  LED_Priority      priority,
                                  LED_On_Status     on_status,
                                  uint16_t          R,
                                  uint16_t          G,
                                  uint16_t          B,
                                  uint32_t          onTimeMS,
                                  uint32_t          offTimeMS,
                                  uint16_t          count)
{
    if (p != NULL)
        return static_cast<LEDIndicate *>(p)->Set(priority, on_status, R, G, B, onTimeMS, offTimeMS, count);
}

void led_indicate_stop(LEDIndicateHandle p)
{
    if (p != NULL)
        static_cast<LEDIndicate *>(p)->Stop();
}

void led_indicate_start(LEDIndicateHandle p)
{
    if (p != NULL)
        static_cast<LEDIndicate *>(p)->Start();
}

void led_indicate_set_done(LEDIndicateHandle p, LED_Priority priority)
{
    if (p != NULL)
        return static_cast<LEDIndicate *>(p)->Set(priority, LED_Off);
}

void led_indicate_set_done_run(LEDIndicateHandle p)
{
    if (p != NULL)
        return static_cast<LEDIndicate *>(p)->Done();
}

void led_indicate_set_mask(LEDIndicateHandle p, LED_Priority priority)
{
    if (p != NULL)
        return static_cast<LEDIndicate *>(p)->SetMask(priority);
}

void led_indicate_cancel_mask(LEDIndicateHandle p, LED_Priority priority)
{
    if (p != NULL)
        return static_cast<LEDIndicate *>(p)->CancelMask(priority);
}

#ifdef __cplusplus
}
#endif
