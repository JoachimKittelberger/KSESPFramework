/**
 * @file KSRTC.h
 * 
 * @brief Header file for KSRTC class
 * 
 * @details 
 * 
 * @see
 *   - adafruit/RTClib @ ^2.1.1
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


#ifndef _KSRTC_H
#define _KSRTC_H


#include <RTClib.h>
#include "KSCriticalSection/src/KSCriticalSection.h"


#define ISO8601_LEN (sizeof("1970-01-01T23:59:59.123456Z"))
extern const char timezoneGermany[];

// lokale Zeitzone definieren
// https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
extern const long gmtOffset_sec;		                  // UTC - 1.00



struct deviceUptime {
    long days;
    long hours;
    long mins;
    long secs;
};


/***** Den Wochentag nach ISO 8601 (1 = Mo, 2 = Di, 3 = Mi, 4 = Do, 5 = Fr, 6 = Sa, 7 = So) berechnen *****/
uint8_t GetWeekday(uint16_t y, uint8_t m, uint8_t d);

/***** Die Wochennummer nach ISO 8601 berechnen *****/
uint8_t GetWeekNumber(uint16_t y, uint8_t m, uint8_t d);

/***** die Anzahl der Tage (Tag des Jahres) berechnen *****/
uint16_t GetDayOfYear(uint16_t y, uint8_t m, uint8_t d);

/***** Testen, ob das Jahr ein Schaltjahr ist *****/
bool IsLeapYear(uint16_t y);




class KSRTC {
    public:
        KSRTC(KSCriticalSection* pcsI2C);
        ~KSRTC();

        bool init(bool bSetCompileTimeIfLost = true);
        static void initTimeZoneGermany();

        void adjust(const DateTime &dt);
        void adjust(tm* ptm);
        DateTime now();
        float getTemperature();
        bool lostPower();

        void getLocalProcessorDateTime(tm* pTimeInfo);
        void setLocalProcessorDateTimeFromRTC();
        void setLocalProcessorDateTime(const DateTime &dt);   
        
        const char* getLocalDateTimeFormated();
        const char* getLocalDateFormated();
        const char* getLocalDateFormatedReverse();
        const char* getLocalTimeFormated();
        String getLocalDateTimeISO8601Formated();

        static String getDateTimeISO8601Formated(int year, int month, int day, int hour, int minute, int second);
        static String getDateTimeISO8601Formated(tm timeinfo);
        static void getDateTimeFromISO8601(char iso8601[ISO8601_LEN], int* pYear, int* pMonth, int* pDay, int* pHour, int* pMinute, int* pSecond);
        static char* datetime2ISO8601(char iso8601[ISO8601_LEN], DateTime& dt);
        static char* timeval2ISO8601(char iso8601[ISO8601_LEN], uint8_t precision, const struct timeval *tv);

        static bool isDateTimeValid(tm* pTimeinfo);
        bool hasHardwareRTC() { return _bHasHWRTC; }

        deviceUptime getDeviceUptime();
        String getDeviceUptimeString();


    private:
        KSCriticalSection* _pcsI2C = NULL;
        
        RTC_DS3231 _rtc;
        bool _bHasHWRTC = false;

        char _szFormatedDateTime[ISO8601_LEN] = "12.07.1969 13:21:25";
        char _szFormatedDate[ISO8601_LEN] = "12.07.1969";
        char _szFormatedTime[ISO8601_LEN] = "13:21:25";
};


#endif  // #define _KSRTC_H