/**
 * @file KSUtilities.h
 * 
 * @brief Header file for using KSUtilities functions
 * 
 * @details 
 * 
 * @see
 *   - https://github.com/dmkishi/Dusk2Dawn.git @ 1.0.1
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

#ifndef _KSUTILITIES_H
#define _KSUTILITIES_H


#include <Arduino.h>


// is now in ESP SDK
//#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__) // For Windows use \\ instead of /

// check, if an string starts with an given prefixstring
//#define STARTS_WITH(string_to_check, prefix) (strncmp(string_to_check, prefix, ((sizeof(prefix) / sizeof(prefix[0])) - 1)) ? 0:((sizeof(prefix) / sizeof(prefix[0])) - 1))
bool strStartsWith(const char *str, const char *prefix);

#define US2SECOND 1000000ULL            // Conversion factor for micro seconds to seconds
#define MS2SECOND 1000UL                // Conversion factor for milli seconds to seconds


// define Macros for loops dependent on cycletime
//	static unsigned long startloopNameMillis = millis();
//	unsigned long currentloopNameMillis = millis();
//	if ((currentloopNameMillis - startloopNameMillis) >= cycletime) {
//		startloopNameMillis = currentloopNameMillis;
//      ...
//	}
#define BEGIN_CYCLIC(loopName, cycleTime) \
  static unsigned long start ## loopName ## Millis = millis(); \
  unsigned long current ## loopName ## Millis = millis(); \
  if ((current ## loopName ## Millis - start ## loopName ## Millis) >= (cycleTime)) { \
    start ## loopName ## Millis = current ## loopName ## Millis;

#define END_CYCLIC() }



void formatDATE2DE(char const *date, char *buff, size_t len);
void printSketchInfo(char const* currentProject);


// CRC-Brechnung für Insta -> Evtl. in Instal-Klasse übernehmen
uint8_t calculateCRCXOR(uint8_t* pBuffer, int count);

char getHexHigh(uint8_t value);
char getHexLow(uint8_t value);


// DateTime Fuctions
int GetNumberOfDaysPerMonth(int month, int year);
int GetDayOfWeekForDate(int day, int month, int year);
int GetLastWeekDayInMonth(int weekDay, int month, int year);

bool isSummerTime(struct tm* pTimeinfo);



extern const float LATITUDE_HERE;           // Latitude value for calculating the sunset and sunrise
extern const float LONGITUDE_HERE;          // Longitude value for calculating the sunset and sunrise
extern const float TIMEZONE_HERE_HOURS;     // Timezone for calculating the sunset and sunrise in hours

int GetSunriseForDateInMin(struct tm* pTimeinfo, bool isDST);
int GetSunsetForDateInMin(struct tm* pTimeinfo, bool isDST);
void printSunSetSunDownForToday();
void printTimeinfoStruct(struct tm* pTimeinfo);

#if !defined(ESP32)
bool getLocalTime(struct tm * info, uint32_t ms = 5000)
#endif

time_t getCurrentTime();
time_t timeInfo2time_t(tm *pTimeInfo);
tm* time_t2TimeInfo(time_t* pTime, tm *pTimeInfo);




#endif // #define _KSUTILITIES_H