#include <stdio.h>
#include "ledc.h"


void app_main(void)
{
    LEDIndicateHandle led0 = init_led_indicate(1);

    led_indicate_set_blink(led0, NON_INDICATOR, 1, 0, 1, 1000, 1000);
    vTaskDelay(pdMS_TO_TICKS(1000));

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    
}
