/**
 * @file KSDeepSleep.h
 * 
 * @brief Header for KSDeepSleep class
 * 
 * @details
 * 
 * @see
 *   - Details: https://randomnerdtutorials.com/esp32-deep-sleep-arduino-ide-wake-up-sources/
 *
 * @author Joachim Kittelberger <jkittelberger@kibesoft.de>
 * @date 28.07.2023
 * @version 1.00
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


#pragma once

#include "Arduino.h"

#include "WiFi.h"

#include <esp_sleep.h>
#include "driver/adc.h"
#include <esp_wifi.h>
#include <esp_bt.h>
#include <esp_bt_main.h>

#include "driver/rtc_io.h"


class KSDeepSleep {
    public:
        KSDeepSleep() {}
        void printWakeupReason();
        bool isWakeupReasonGPIO();
        bool isWakeupReasonReset();

        void init(uint64_t time_in_us, gpio_num_t gpio_num = GPIO_NUM_MAX, int level = 1);
        void setWakeupTimer(uint64_t time_in_us);
        uint64_t getWakeupTimer() { return _lastWakeupTimer_in_us; }
        bool sleep();

        void disableBluetooth();
        void disableADC();
        void enableADC();
        void disableWiFi();
        void disableROMLogging();

        // disable deepSleep if OTA ist active
        void setOTAActive(bool active = true) { _disableBecauseOTAActive = active; }

        void setOnBeforeDeepSleepListener(void (*callback)()) { onBeforeDeepSleepListener = callback; }
    
    protected:
       void (*onBeforeDeepSleepListener)();

    private:
        uint64_t _lastWakeupTimer_in_us = 0;

        bool _disableBecauseOTAActive = false;
};




