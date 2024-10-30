#ifndef __CTRL_LED_H__
#define __CTRL_LED_H__

#include "led.h"

#ifdef __cplusplus

struct LED_Element
{
    LED_Priority  priority;
    LED_On_Status on_status;
    uint8_t       blink_count;
    uint16_t      R;
    uint16_t      G;
    uint16_t      B;
    uint32_t      onTimeMS;  // It only works when the on_status is blink
    uint32_t      offTimeMS; // It only works when the on_status is blink

    bool operator!=(const LED_Element &led)
    {
        if (priority != led.priority || on_status != led.on_status || blink_count != led.blink_count || R != led.R ||
            G != led.G || B != led.B || onTimeMS != led.onTimeMS || offTimeMS != led.offTimeMS)
            return true;
        return false;
    }

    bool operator==(const LED_Element &led)
    {
        if (priority != led.priority || on_status != led.on_status || blink_count != led.blink_count || R != led.R ||
            G != led.G || B != led.B || onTimeMS != led.onTimeMS || offTimeMS != led.offTimeMS)
            return false;
        return true;
    }
};

class LEDIndicate
{
private:
    struct LEDStack
    {
        int         top;
        LED_Element led_element[LEDPriorityCount + 1];
    } led_stack;
    bool LedMask[LEDPriorityCount];
    LED               mled;
    SemaphoreHandle_t ledxSemaphore;
    bool              IsStop;

    void LEDPriorityDispatch(LED_Priority  priority,
                             LED_On_Status on_status,
                             uint16_t      R,
                             uint16_t      G,
                             uint16_t      B,
                             uint32_t      onTimeMS,
                             uint32_t      offTimeMS,
                             uint16_t      count = 0);
    void LEDHandleOnBlink(LED_Priority  priority,
                          LED_On_Status on_status,
                          uint16_t      R,
                          uint16_t      G,
                          uint16_t      B,
                          uint32_t      onTimeMS,
                          uint32_t      offTimeMS,
                          uint16_t      count = 0);
    void LEDHandleOff(LED_Priority priority, LED_On_Status on_status);
    void RunStackTop(void);
    bool CheckIsMask(LED_Priority priority);

public:
    LEDIndicate(LED_t);
    ~LEDIndicate();
    LEDIndicate(const LEDIndicate &) = delete;
    void Set(LED_Priority  priority,
             LED_On_Status on_status,
             uint16_t      R         = 0,
             uint16_t      G         = 0,
             uint16_t      B         = 0,
             uint32_t      onTimeMS  = 0,
             uint32_t      offTimeMS = 0,
             uint16_t      count     = 0);
    void Stop(void);
    void Start(void);
    void Done(void);
    void SetMask(LED_Priority priority);
    void CancelMask(LED_Priority priority);
};

#endif

#endif
