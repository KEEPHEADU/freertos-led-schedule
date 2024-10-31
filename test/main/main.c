#include "esp_log.h"
#include "led_driver.h"
#include "ledc.h"
#include <stdio.h>

typedef enum {
  NON_INDICATOR,
  ETH_INDICATOR,
  WIFI_INDICATOR,
  RESET_INDICATOR,
  CONFIG_INDICATOR
} mLEDPriority; // The maximum priority defined by MAX_NUMBER_OF_LED_PRIORITY

void led0_callback(bool state, uint16_t R, uint16_t G, uint16_t B) {
  ESP_LOGI("LED-0", "state: %d  R:%d G:%d B:%d", state, R, G, B);

  /* Use the parameter information in the callback to use the function that
   * controls the led */
  set_led(state, R, G, B);
}

void led1_callback(bool state, uint16_t R, uint16_t G, uint16_t B) {
  ESP_LOGW("LED-1", "state: %d  R:%d G:%d B:%d", state, R, G, B);

  /* In this place, the led is controlled according to the incoming parameters
   */
}

void led2_callback(bool state, uint16_t R, uint16_t G, uint16_t B) {
  ESP_LOGE("LED-2", "state: %d  R:%d G:%d B:%d", state, R, G, B);

  /* In this place, the led is controlled according to the incoming parameters
   */
}

void app_main(void) {
  /* Using this component requires you to implement the initialization of the
   * led and implement the control of the led callback */

  /* Configure the peripheral according to the LED type */
  configure_led();

  /* Initializing the led component */
  /* Three levels of log are used to represent the state changes of different
   * leds*/
  /* The number of leds should be less than MAX_NUMBER_OF_LED */
  LEDIndicateHandle led0 = init_led_indicate(led0_callback); // ESP_LOGI green
  LEDIndicateHandle led1 = init_led_indicate(led1_callback); // ESP_LOGW yellow
  LEDIndicateHandle led2 = init_led_indicate(led2_callback); // ESP_LOGE red
  ESP_LOGI("LED", "------------------------led init done!!!\n\n");

  led_indicate_set_blink(led0, NON_INDICATOR, 10, 10, 10, 500, 1000);
  led_indicate_set_blink(led1, NON_INDICATOR, 1, 0, 1, 1000, 1000);
  led_indicate_set_blink(led2, NON_INDICATOR, 1, 0, 1, 1000, 1000);

  ESP_LOGW("LED1", "------------------------Set led2 NON_INDICATOR done!!");
  led_indicate_set_done(led1, NON_INDICATOR);
  vTaskDelay(pdMS_TO_TICKS(5000));

  ESP_LOGE("LED2", "------------------------Set led2 NON_INDICATOR done!!");
  led_indicate_set_done(led2, NON_INDICATOR);
  vTaskDelay(pdMS_TO_TICKS(2000));

  ESP_LOGI("LED0", "------------------------Set led0 NON_INDICATOR done!!");
  led_indicate_set_done(led0, NON_INDICATOR);
  vTaskDelay(pdMS_TO_TICKS(1000));

  ESP_LOGI("LED", "------------------------Start priority test!!\n");

  ESP_LOGI("LED0",
           "------------------------Set led0 NON_INDICATOR priority:%d on!!",
           NON_INDICATOR);
  led_indicate_set(led0, NON_INDICATOR, LED_ON, 10, 10, 10); // Weak white light
  vTaskDelay(pdMS_TO_TICKS(2000));

  ESP_LOGI("LED0",
           "------------------------Set led0 WIFI_INDICATOR priority:%d on!!",
           WIFI_INDICATOR);
  led_indicate_set(led0, WIFI_INDICATOR, LED_ON, 100, 0, 0); // red light
  vTaskDelay(pdMS_TO_TICKS(2000));

  ESP_LOGI("LED0",
           "------------------------Set led0 ETH_INDICATOR priority:%d on!!",
           ETH_INDICATOR);
  led_indicate_set(led0, ETH_INDICATOR, LED_ON, 0, 100, 0); // green light
  vTaskDelay(pdMS_TO_TICKS(2000));

  ESP_LOGI(
      "LED0",
      "------------------------Set led0 WIFI_INDICATOR priority:%d blink!!",
      WIFI_INDICATOR);
  led_indicate_set_blink(led0, WIFI_INDICATOR, 100, 0, 100, 500,
                         500); // purple light

  ESP_LOGI("LED0",
           "------------------------Set led0 ETH_INDICATOR priority:%d done!!",
           ETH_INDICATOR);
  led_indicate_set_done(led0, ETH_INDICATOR); // off green light
  vTaskDelay(pdMS_TO_TICKS(3000));

  ESP_LOGI("LED0",
           "------------------------Set led0 WIFI_INDICATOR priority:%d count "
           "blink 3' !!",
           WIFI_INDICATOR);
  led_indicate_set_count_blink(led0, WIFI_INDICATOR, 100, 100, 0, 500, 500,
                               3); // off yellow light after blink 3 times
  vTaskDelay(pdMS_TO_TICKS(5000));

  ESP_LOGI("LED0",
           "------------------------Set led0 NON_INDICATOR priority:%d done!!",
           NON_INDICATOR);
  led_indicate_set_done(led0, NON_INDICATOR); // off weak white light

  while (1) {
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}
