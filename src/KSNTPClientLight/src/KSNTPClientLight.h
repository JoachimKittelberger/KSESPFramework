/**
 * @file KSNTPClientLight.h
 * 
 * @brief Header file for KSNTPClientLight class
 * 
 * @details 
 * 
 * @see
 *   - https://github.com/PaulStoffregen/Time.git
 * 
 * @author Joachim Kittelberger <jkittelberger@kibesoft.de>
 * @date 20.06.2022
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
#ifndef _KSNTPCLIENTLIGHT_H
#define _KSNTPCLIENTLIGHT_H


#if defined(ESP8266)
    //#pragma message "ESP8266 is defined"
    #include <ESP8266WiFi.h>
#elif defined(ESP32)
    //#pragma message "ESP32 is defined"
    #include <WiFi.h>
#else
#error "This isn't a ESP8266 or ESP32!"
#endif


#include "KSRTC/src/KSRTC.h"
#include "KSUtilities/src/KSUtilities.h"


#define NTPLIGHT_SYNC_INTERVAL_IN_MIN 24*60          // synchronization-interval
//#define NTPLIGHT_SYNC_INTERVAL_IN_MIN 2            // synchronization-interval for testing

extern const char ntpServer[];
extern const char timezoneGermany[];




class KSNTPClientLight {
    public:
        KSNTPClientLight() {}
        ~KSNTPClientLight() {}

        void initTimeZone();                // muss direkt in setup aufgerufen werden
        bool sync(uint32_t timeoutMS);      // sync with ntp server
    protected:
        void getTime(tm* pTimeInfo);        // get local time



/*

        TaskHandle_t createConnection(EventGroupHandle_t *phEventGroupNetwork = NULL, KSRTC* pRTC = NULL);
        void waitForSync();
        bool isInSync() { return _bIsInSync; }

    private:
        void tKSNTPClientLight();

        bool isAdjusted() { return _bTimeAdjusted; }

        bool _bTimeAdjusted = false;        // has minimum one synchronization
        bool _bIsInSync = false;            // is in Sync in the needed syncinterval NTP_SYNC_INTERVAL_IN_MIN
        KSRTC* _pRTC = NULL;                // Hardware RTC or SW-Lib

        TaskHandle_t _htKSNTPClientLight;
        EventGroupHandle_t *_phEventGroupNetwork = NULL;
        */
};




#endif  // #define _KSNTPCLIENTLIGHT_H