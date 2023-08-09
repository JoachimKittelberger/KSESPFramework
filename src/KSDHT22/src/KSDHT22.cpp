/**
 * @file KSDHT22.cpp
 * 
 * @brief Implementation for KSDHT22 class
 * 
 * @details
 * 
 * @see
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

#if defined USE_KSDHT22 || defined KSLIBRARIES_USEALL     // include File in Build only if it ist defined to use it

#include "KSDHT22.h"

#include "KSLogger/src/KSLogger.h"


KSDHT22::KSDHT22(int8_t pin) : _dht(pin, DHTTYPE) {
}



void KSDHT22::init() {
	_dht.begin();
}



bool KSDHT22::read(float* pTemperature, float* pHumidity, float* pHeatindex) {
    bool bRetVal = true;

    if (!pTemperature && !pHumidity && !pHeatindex) return false;

    if (pTemperature) {
        *pTemperature = readTemperature();
        if (isnan(*pTemperature)) {
    		LOGGER.println(F("Failed to read from DHT sensor!"));
            bRetVal = false;
        }
    }

    if (pHumidity) {
        *pHumidity = readHumidity();
        if (isnan(*pHumidity)) {
    		LOGGER.println(F("Failed to read from DHT sensor!"));
            bRetVal = false;
        }
    }

   if (pHeatindex) {
        *pHeatindex = readcomputedHeatIndex();
        if (isnan(*pHeatindex)) {
    		LOGGER.println(F("Failed to read from DHT sensor!"));
            bRetVal = false;
        }
    }

    return bRetVal;
}


#endif      // #if defined USE_KSDHT22 || defined KSLIBRARIES_USEALL