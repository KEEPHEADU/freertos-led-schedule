#ifndef __LED_H__
#define __LED_H__

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/semphr.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Maximum number of LED instances that can be registered */
#define MAX_NUMBER_OF_LED 10

/* Maximum number of LED priorities */
#define MAX_NUMBER_OF_LED_PRIORITY 30

typedef uint16_t LEDPriority;
typedef void *LEDIndicateHandle;
typedef void (*LEDIndicateCallback)(bool state, uint16_t R, uint16_t G,
                                    uint16_t B);

typedef struct {
  void *Obj;
  TimerHandle_t led_timer;
} LED_Obj_Elem;

typedef enum { LED_OFF, LED_ON, LED_Blink, LED_Count_Blink } LEDOnStatus;

#ifdef __cplusplus

class LED {
public:
  ~LED();
  void Init(LEDIndicateHandle led, LEDIndicateCallback callback);
  void Set(bool state, uint16_t R, uint16_t G, uint16_t B);
  void Invert(void);
  void Blink(uint32_t changeRateMS, uint16_t R, uint16_t G, uint16_t B,
             uint16_t blink_count = 0);
  void Blink(uint32_t onTimeMS, uint32_t offTimeMS, uint16_t R, uint16_t G,
             uint16_t B, uint16_t blink_count = 0);
  void StopCountBlink(void);

  static void *GetLEDObj(TimerHandle_t timer);
  static void DeleteLEDObj(TimerHandle_t timer);

  uint32_t GetmBlinkOnTimeMS() { return mBlinkOnTimeMS; }

  uint32_t GetmBlinkOffTimeMS() { return mBlinkOffTimeMS; }

  bool GetLedStatus() { return mLedStatus; }

  bool GetIsCountBlink() { return mIsCountBlink; }

  void SetIsCountBlink(bool bli) { mIsCountBlink = bli; }

  uint16_t GetCountBlink() { return mblink_count; }

  void SetCountBlink(uint16_t _blink_count) { mblink_count = _blink_count; }

  LEDIndicateHandle GetmLed() { return mLed; }

private:
  static LED_Obj_Elem led_obj_list[MAX_NUMBER_OF_LED];
  static uint8_t led_obj_count;
  uint32_t mBlinkOnTimeMS;
  uint32_t mBlinkOffTimeMS;
  uint16_t mR;
  uint16_t mG;
  uint16_t mB;
  LEDIndicateHandle mLed;
  bool mLedStatus;
  bool mIsCountBlink;
  uint8_t mblink_count;
  TimerHandle_t led_timer;
  LEDIndicateCallback mcallback;

  void SetLed(bool state, uint16_t R, uint16_t G, uint16_t B) {
    if (mcallback != nullptr) {
      mcallback(state, R, G, B);
    }
  }
};
#endif

#endif
