#ifndef __CTRL_LED_H__
#define __CTRL_LED_H__

#include "led.h"

#ifdef __cplusplus

struct LED_Element {
  LEDPriority priority;
  LEDOnStatus on_status;
  uint8_t blink_count;
  uint16_t R;
  uint16_t G;
  uint16_t B;
  uint32_t onTimeMS;  // It only works when the on_status is blink
  uint32_t offTimeMS; // It only works when the on_status is blink

  bool operator!=(const LED_Element &led) {
    if (priority != led.priority || on_status != led.on_status ||
        blink_count != led.blink_count || R != led.R || G != led.G ||
        B != led.B || onTimeMS != led.onTimeMS || offTimeMS != led.offTimeMS)
      return true;
    return false;
  }

  bool operator==(const LED_Element &led) {
    if (priority != led.priority || on_status != led.on_status ||
        blink_count != led.blink_count || R != led.R || G != led.G ||
        B != led.B || onTimeMS != led.onTimeMS || offTimeMS != led.offTimeMS)
      return false;
    return true;
  }
};

class LEDIndicate {
private:
  struct LEDStack {
    int top;
    LED_Element led_element[MAX_NUMBER_OF_LED_PRIORITY + 1];
  } led_stack;
  bool LedMask[MAX_NUMBER_OF_LED_PRIORITY];
  LED mled;
  SemaphoreHandle_t ledxSemaphore;
  bool IsStop;

  void LEDPriorityDispatch(LEDPriority priority, LEDOnStatus on_status,
                           uint16_t R, uint16_t G, uint16_t B,
                           uint32_t onTimeMS, uint32_t offTimeMS,
                           uint16_t count = 0);
  void LEDHandleOnBlink(LEDPriority priority, LEDOnStatus on_status, uint16_t R,
                        uint16_t G, uint16_t B, uint32_t onTimeMS,
                        uint32_t offTimeMS, uint16_t count = 0);
  void LEDHandleOff(LEDPriority priority, LEDOnStatus on_status);
  void RunStackTop(void);
  bool CheckIsMask(LEDPriority priority);

public:
  LEDIndicate(LEDIndicateCallback);
  ~LEDIndicate();
  LEDIndicate(const LEDIndicate &) = delete;
  void Set(LEDPriority priority, LEDOnStatus on_status, uint16_t R = 0,
           uint16_t G = 0, uint16_t B = 0, uint32_t onTimeMS = 0,
           uint32_t offTimeMS = 0, uint16_t count = 0);
  void Stop(void);
  void Start(void);
  void Done(void);
  void SetMask(LEDPriority priority);
  void CancelMask(LEDPriority priority);
};

#endif

#endif
