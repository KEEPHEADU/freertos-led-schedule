#include "led.h"
#include "ledc.h"

uint8_t LED::led_obj_count = 0;
LED_Obj_Elem LED::led_obj_list[MAX_NUMBER_OF_LED] = {};

void InitLEDTimer(TimerHandle_t &timer);

LED::~LED() {
  LED::DeleteLEDObj(led_timer);
  xTimerStop(led_timer, 0);
  xTimerDelete(led_timer, 0);
}

void LED::Init(const LEDIndicateHandle led, LEDIndicateCallback callback) {
  mBlinkOnTimeMS = 0;
  mBlinkOffTimeMS = 0;
  mLed = led;
  mLedStatus = false;
  mblink_count = 0;
  mIsCountBlink = false;
  mcallback = callback;
  InitLEDTimer(led_timer);

  LED::led_obj_list[LED::led_obj_count].Obj = this;
  LED::led_obj_list[LED::led_obj_count++].led_timer = led_timer;

  Set(false, 0, 0, 0);
}

void LED::Invert(void) {
  mLedStatus = !mLedStatus;
  SetLed(mLedStatus, mR, mG, mB);
}

void LED::Set(bool state, uint16_t R, uint16_t G, uint16_t B) {
  xTimerStop(led_timer, pdMS_TO_TICKS(1000));
  mR = R;
  mG = G;
  mB = B;
  mBlinkOnTimeMS = mBlinkOffTimeMS = 0;
  SetLed(state, R, G, B);
  mLedStatus = state;
}

void LED::Blink(uint32_t changeRateMS, uint16_t R, uint16_t G, uint16_t B,
                uint16_t _blink_count) {
  Blink(changeRateMS, changeRateMS, R, G, B, _blink_count);
}

void LED::Blink(uint32_t onTimeMS, uint32_t offTimeMS, uint16_t R, uint16_t G,
                uint16_t B, uint16_t _blink_count) {
  xTimerStop(led_timer, pdMS_TO_TICKS(1000));
  mR = R;
  mG = G;
  mB = B;
  mBlinkOnTimeMS = onTimeMS;
  mBlinkOffTimeMS = offTimeMS;
  if (_blink_count != 0) {
    mIsCountBlink = true;
    mblink_count = _blink_count;
  } else {
    mIsCountBlink = false;
    mblink_count = _blink_count;
  }

  Invert();
  xTimerChangePeriod(
      led_timer, pdMS_TO_TICKS(mLedStatus ? mBlinkOnTimeMS : mBlinkOffTimeMS),
      pdMS_TO_TICKS(1000));
  xTimerReset(led_timer, pdMS_TO_TICKS(1000));
}

void LED::StopCountBlink(void) {
  if (!mIsCountBlink)
    return;

  Set(false, 0, 0, 0);
  mIsCountBlink = false;
  mblink_count = 0;
}

void _led_timer_handler(TimerHandle_t xTimer) {
  LED_Obj_Elem *obj = ((LED_Obj_Elem *)LED::GetLEDObj(xTimer));

  if (((LED *)obj->Obj)->GetIsCountBlink() &&
      !(((LED *)obj->Obj)->GetLedStatus()) &&
      ((LED *)obj->Obj)->GetCountBlink()) {
    ((LED *)obj->Obj)->SetCountBlink(((LED *)obj->Obj)->GetCountBlink() - 1);
    if (((LED *)obj->Obj)->GetCountBlink() == 0) {
      ((LED *)obj->Obj)->SetIsCountBlink(false);
      led_indicate_set_done_run(((LED *)obj->Obj)->GetmLed());
      return;
    }
  }

  ((LED *)obj->Obj)->Invert();
  if (((LED *)obj->Obj)->GetLedStatus()) {
    xTimerStop(obj->led_timer, pdMS_TO_TICKS(1000));
    xTimerChangePeriod(obj->led_timer,
                       pdMS_TO_TICKS(((LED *)obj->Obj)->GetmBlinkOnTimeMS()),
                       pdMS_TO_TICKS(1000));
    xTimerReset(obj->led_timer, pdMS_TO_TICKS(1000));
  } else {
    xTimerStop(obj->led_timer, pdMS_TO_TICKS(1000));
    xTimerChangePeriod(obj->led_timer,
                       pdMS_TO_TICKS(((LED *)obj->Obj)->GetmBlinkOffTimeMS()),
                       pdMS_TO_TICKS(1000));
    xTimerReset(obj->led_timer, pdMS_TO_TICKS(1000));
  }
}

void InitLEDTimer(TimerHandle_t &_led_timer) {
  _led_timer = xTimerCreate("led_timer", pdMS_TO_TICKS(1000), pdFALSE,
                            (void *)0, _led_timer_handler);
}

void *LED::GetLEDObj(TimerHandle_t timer) {
  if (led_obj_count == 0) {
    return nullptr;
  }

  uint8_t i;
  for (i = 0; i < led_obj_count && led_obj_list[i].led_timer != timer; i++)
    ;

  if (i == led_obj_count) {
    return nullptr;
  } else {
    return &led_obj_list[i];
  }
}

void LED::DeleteLEDObj(TimerHandle_t timer) {
  if (led_obj_count == 0) {
    return;
  }

  uint8_t i;
  for (i = 0; i < led_obj_count && led_obj_list[i].led_timer != timer; i++)
    ;

  if (i == led_obj_count) {
    return;
  }

  for (uint8_t j = i; j < led_obj_count; j++) {
    led_obj_list[j] = led_obj_list[j + 1];
  }
  led_obj_count--;
}
