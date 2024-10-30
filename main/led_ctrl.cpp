#include "led_ctrl.h"

void LEDIndicate::Done(void)
{
    xSemaphoreTake(ledxSemaphore, pdMS_TO_TICKS(1000));
    LEDPriorityDispatch(led_stack.led_element[led_stack.top].priority, LED_Off, 0, 0, 0, 0, 0);
    RunStackTop();
    xSemaphoreGive(ledxSemaphore);
}

void LEDIndicate::Set(LED_Priority  priority,
                      LED_On_Status on_status,
                      uint16_t      R,
                      uint16_t      G,
                      uint16_t      B,
                      uint32_t      onTimeMS,
                      uint32_t      offTimeMS,
                      uint16_t      count)
{
    xSemaphoreTake(ledxSemaphore, pdMS_TO_TICKS(1000));
    LED_Element top_elem = led_stack.top < 0 ? LED_Element{} : led_stack.led_element[led_stack.top];

    LEDPriorityDispatch(priority, on_status, R, G, B, onTimeMS, offTimeMS, count);
    
    if(top_elem == LED_Element{} || led_stack.top < 0 || top_elem != led_stack.led_element[led_stack.top]) {
        RunStackTop();
    }
    
    xSemaphoreGive(ledxSemaphore);
}

void LEDIndicate::LEDPriorityDispatch(LED_Priority  priority,
                                      LED_On_Status on_status,
                                      uint16_t      R,
                                      uint16_t      G,
                                      uint16_t      B,
                                      uint32_t      onTimeMS,
                                      uint32_t      offTimeMS,
                                      uint16_t      count)
{
    switch (on_status) {
    case LED_On:
    case LED_Blink:
    case LED_Count_Blink:
        LEDHandleOnBlink(priority, on_status, R, G, B, onTimeMS, offTimeMS, count);
        break;

    case LED_Off:
        LEDHandleOff(priority, on_status);
        break;

    default:
        break;
    }
}

void LEDIndicate::LEDHandleOnBlink(LED_Priority  priority,
                                   LED_On_Status on_status,
                                   uint16_t      R,
                                   uint16_t      G,
                                   uint16_t      B,
                                   uint32_t      onTimeMS,
                                   uint32_t      offTimeMS,
                                   uint16_t      count)
{
    if (led_stack.top < 0) {
        led_stack.top++;
        led_stack.led_element[led_stack.top].on_status   = on_status;
        led_stack.led_element[led_stack.top].priority    = priority;
        led_stack.led_element[led_stack.top].onTimeMS    = onTimeMS;
        led_stack.led_element[led_stack.top].offTimeMS   = offTimeMS;
        led_stack.led_element[led_stack.top].R           = R;
        led_stack.led_element[led_stack.top].G           = G;
        led_stack.led_element[led_stack.top].B           = B;
        led_stack.led_element[led_stack.top].blink_count = count;
    }
    else {
        uint8_t i = 0;
        for (; i < LEDPriorityCount && i <= led_stack.top && led_stack.led_element[i].priority < priority; i++)
            ;
        if (i <= led_stack.top && led_stack.led_element[i].priority == priority) {
            led_stack.led_element[i].on_status   = on_status;
            led_stack.led_element[i].onTimeMS    = onTimeMS;
            led_stack.led_element[i].offTimeMS   = offTimeMS;
            led_stack.led_element[i].R           = R;
            led_stack.led_element[i].G           = G;
            led_stack.led_element[i].B           = B;
            led_stack.led_element[i].blink_count = count;
        }
        else if (i < LEDPriorityCount) {
            for (int8_t j = led_stack.top; j >= i; --j) {
                led_stack.led_element[j + 1] = led_stack.led_element[j];
            }
            led_stack.led_element[i].on_status   = on_status;
            led_stack.led_element[i].priority    = priority;
            led_stack.led_element[i].onTimeMS    = onTimeMS;
            led_stack.led_element[i].offTimeMS   = offTimeMS;
            led_stack.led_element[i].R           = R;
            led_stack.led_element[i].G           = G;
            led_stack.led_element[i].B           = B;
            led_stack.led_element[i].blink_count = count;
            led_stack.top++;
        }
    }
}

void LEDIndicate::LEDHandleOff(LED_Priority priority, LED_On_Status on_status)
{
    if (led_stack.top < 0) {
        return;
    }
    uint8_t i = 0;
    while (i < LEDPriorityCount && i <= led_stack.top && led_stack.led_element[i].priority != priority) i++;
    if (i == LEDPriorityCount || i > led_stack.top) {
        return;
    }

    for (uint8_t j = i; j <= led_stack.top; j++) {
        led_stack.led_element[j] = led_stack.led_element[j + 1];
    }
    led_stack.top--;
}

void LEDIndicate::RunStackTop(void)
{
    if (led_stack.top < 0) {
        mled.Set(false, 0, 0, 0);
        return;
    }

    if(CheckIsMask(led_stack.led_element[led_stack.top].priority))
    {
        LEDPriorityDispatch(led_stack.led_element[led_stack.top].priority, LED_Off, 0, 0, 0, 0, 0);
        RunStackTop();
    }

    switch (led_stack.led_element[led_stack.top].on_status) {
    case LED_On:
        mled.Set(true, led_stack.led_element[led_stack.top].R, led_stack.led_element[led_stack.top].G,
                 led_stack.led_element[led_stack.top].B);
        break;

    case LED_Blink:
    case LED_Count_Blink:
        mled.Blink(led_stack.led_element[led_stack.top].onTimeMS, led_stack.led_element[led_stack.top].offTimeMS,
                   led_stack.led_element[led_stack.top].R, led_stack.led_element[led_stack.top].G,
                   led_stack.led_element[led_stack.top].B, led_stack.led_element[led_stack.top].blink_count);
        break;

    case LED_Off:
        mled.Set(false, 0, 0, 0);
        break;

    default:
        break;
    }
}

void LEDIndicate::Stop(void)
{
    if (IsStop == true)
        return;
    xSemaphoreTake(ledxSemaphore, pdMS_TO_TICKS(1000));
    IsStop = true;
    mled.Set(false, 0, 0, 0);
    xSemaphoreGive(ledxSemaphore);
}

void LEDIndicate::Start(void)
{
    if (IsStop == false)
        return;
    xSemaphoreTake(ledxSemaphore, pdMS_TO_TICKS(1000));
    IsStop = false;
    RunStackTop();
    xSemaphoreGive(ledxSemaphore);
}


void LEDIndicate::SetMask(LED_Priority priority)
{
    xSemaphoreTake(ledxSemaphore, pdMS_TO_TICKS(1000));
    LedMask[priority] = true;
    if(led_stack.top >= 0 && led_stack.led_element[led_stack.top].priority == priority) {
        RunStackTop();
    }
    xSemaphoreGive(ledxSemaphore);
}

void LEDIndicate::CancelMask(LED_Priority priority)
{
    xSemaphoreTake(ledxSemaphore, pdMS_TO_TICKS(1000));
    LedMask[priority] = false;
    xSemaphoreGive(ledxSemaphore);
}

bool LEDIndicate::CheckIsMask(LED_Priority priority)
{
    return LedMask[priority];
}

LEDIndicate::LEDIndicate(LED_t led)
{
    IsStop = false;
    mled.Init(this);
    led_stack.top = -1;
    memset(LedMask, 0, sizeof(LedMask));
    ledxSemaphore = xSemaphoreCreateMutex();
}

LEDIndicate::~LEDIndicate()
{
}
