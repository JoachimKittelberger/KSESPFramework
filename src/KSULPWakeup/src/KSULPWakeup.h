/**
 * @file KSULPWakeup.h
 * 
 * @brief Definition of KSULPWakeup functions
 * 
 * @details
 * 
 * @see
 * 
 * @author Joachim Kittelberger <jkittelberger@kibesoft.de>
 * @date 24.04.2022
 * @version 1.01
 *
 * @todo
 *
 * @bug
 */

/**
 * @copyright
 * MIT License
 *
 * Copyright (c) 2023 Joachim Kittelberger - KibeSoft, www.kibesoft.de
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _KSULPWAKEUP_H
#define _KSULPWAKEUP_H


#include <esp_sleep.h>
#include <esp32/rom/uart.h>
#include <soc/rtc.h>
#include <driver/rtc_io.h>




// function prototypes
RTC_IRAM_ATTR void ulpSetDeepSleepWakeupTimerForUs(uint64_t duration_us);
RTC_IRAM_ATTR esp_err_t ulpSetExt0WakeupPin(gpio_num_t gpio_num, int level);

RTC_IRAM_ATTR uint64_t ulpRtcTimeGetClockCycles(void);
RTC_IRAM_ATTR void ulpStartDeepSleep(uint32_t wakeupMask);
RTC_IRAM_ATTR uint64_t ulpRtcTimeGetUs(void);
RTC_IRAM_ATTR void ulpSetWakeupStub(void (*wakeupStubfunc)());

RTC_IRAM_ATTR void ulpFeedWatchdog(void);
RTC_IRAM_ATTR void ulpWaitUARTTXEnd(uint32_t uartNumber);

RTC_IRAM_ATTR bool ulpDigitalRead(uint8_t pin);




#endif          // #define _KSULPWAKEUP_H