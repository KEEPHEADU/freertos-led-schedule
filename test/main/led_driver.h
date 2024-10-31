#ifndef __LED_DRIVER__
#define __LED_DRIVER__

#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led_strip.h"
#include "sdkconfig.h"
#include <stdio.h>

void set_led(bool on_off, uint16_t r, uint16_t g, uint16_t b);
void configure_led(void);

#endif
