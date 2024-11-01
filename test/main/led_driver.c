#include "led_driver.h"

static const char *TAG = "example";

/* Use project configuration menu (idf.py menuconfig) to choose the GPIO to
   blink, or you can edit the following line and set a number here.
*/
#define BLINK_GPIO CONFIG_BLINK_GPIO

#ifdef CONFIG_BLINK_LED_RMT

static led_strip_handle_t led_strip;

void set_led(bool on_off, uint16_t r, uint16_t g, uint16_t b) {
  /* If the addressable LED is enabled */
  if (on_off) {
    /* Set the LED pixel using RGB from 0 (0%) to 255 (100%) for each color */
    led_strip_set_pixel(led_strip, 0, r, g, b);
    /* Refresh the strip to send data */
    led_strip_refresh(led_strip);
  } else {
    /* Set all LED off to clear all pixels */
    led_strip_clear(led_strip);
  }
}

void configure_led(void) {
  ESP_LOGI(TAG, "Example configured to blink addressable LED!");
  /* LED strip initialization with the GPIO and pixels number*/
  led_strip_config_t strip_config = {
      .strip_gpio_num = BLINK_GPIO,
      .max_leds = 1, // at least one LED on board
  };
  led_strip_rmt_config_t rmt_config = {
      .resolution_hz = 10 * 1000 * 1000, // 10MHz
  };
  ESP_ERROR_CHECK(
      led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));
  /* Set all LED off to clear all pixels */
  led_strip_clear(led_strip);
}

#elif CONFIG_BLINK_LED_GPIO

void set_led(bool on_off, uint16_t r, uint16_t g, uint16_t b) {
  /* Set the GPIO level according to the state (LOW or HIGH)*/
  gpio_set_level(BLINK_GPIO, on_off);
}

void configure_led(void) {
  ESP_LOGI(TAG, "Example configured to blink GPIO LED!");
  gpio_reset_pin(BLINK_GPIO);
  /* Set the GPIO as a push/pull output */
  gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
}

#endif
