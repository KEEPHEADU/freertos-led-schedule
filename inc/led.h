#ifndef __LED_H__
#define __LED_H__

#include "ledconfig.h"

typedef void *LEDIndicateHandle;

typedef struct
{
    void         *Obj;
    TimerHandle_t led_timer;
} LED_Obj_Elem;

#ifdef __cplusplus

class LED
{
public:
    ~LED();
    void Init(LEDIndicateHandle led);
    void Set(bool state, uint16_t R, uint16_t G, uint16_t B);
    void Invert(void);
    void Blink(uint32_t changeRateMS, uint16_t R, uint16_t G, uint16_t B, uint16_t blink_count = 0);
    void Blink(uint32_t onTimeMS, uint32_t offTimeMS, uint16_t R, uint16_t G, uint16_t B, uint16_t blink_count = 0);
    void StopCountBlink(void);

    static void *Get_LED_Obj(TimerHandle_t timer);
    static void  Delete_LED_Obj(TimerHandle_t timer);

    uint32_t GetmBlinkOnTimeMS() { return mBlinkOnTimeMS; }

    uint32_t GetmBlinkOffTimeMS() { return mBlinkOffTimeMS; }

    bool GetLedStatus() { return mLedStatus; }

    bool GetIsCountBlink() { return mIsCountBlink; }

    void SetIsCountBlink(bool bli) { mIsCountBlink = bli; }

    uint16_t GetCountBlink() { return mblink_count; }

    void SetCountBlink(uint16_t _blink_count) { mblink_count = _blink_count; }

    LEDIndicateHandle GetmLed() {return mLed;}

private:
    static LED_Obj_Elem LED_Obj_List[LED_COUNT];
    static uint8_t      led_obj_count;
    uint32_t            mBlinkOnTimeMS;
    uint32_t            mBlinkOffTimeMS;
    uint16_t            mR;
    uint16_t            mG;
    uint16_t            mB;
    LEDIndicateHandle   mLed;
    bool                mLedStatus;
    bool                mIsCountBlink;
    uint8_t             mblink_count;
    TimerHandle_t       led_timer;

    void SetLed(bool state, uint16_t R, uint16_t G, uint16_t B) { LED_SET(state, mLed, R, G, B); }
};
#endif
#endif
