#ifndef __LEDCONFIG_H__
#define __LEDCONFIG_H__

//////////////////////////////////////////////////
// Add headers for the current platform freertos
// and headers required by the LED_SET function
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include "esp_log.h"

//////////////////////////////////////////////////

#define LED_DEBUG 0

// Number of leds
#define LED_COUNT 5

// The number of priorities, corresponding to the number of enumerated variables in LED_Priority
#define LEDPriorityCount 12

// A high priority will preempt a low priority
typedef enum
{
    NON_INDICATOR,
    ETH_INDICATOR,
    WIFI_INDICATOR,
    RESET_INDICATOR,
    CONFIG_INDICATOR
} LED_Priority;

// LED status, nothing need to be changed
typedef enum
{
    LED_Off,
    LED_On,
    LED_Blink,
    LED_Breath,
    LED_Count_Blink
} LED_On_Status;

// You need to define LED_t as the LED handle for the current platform
typedef int LED_t;

// You need to implement functions that control leds based on led handles
#define LED_SET(state, handle, R, G, B)                             \
    do {                                                            \
        ESP_LOGI("LED INDICATOR", "state: %d  R:%d G:%d B:%d", state, R, G, B);\
    } while (0)

#define LED_GPIO_INIT()                                                      \
    do {                                                                     \
    } while (0)

#endif
