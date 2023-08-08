/**
 * @file KSTEMT6000.cpp
 * 
 * @brief Implementation of KSTEMT6000 class
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

#if defined USE_KSTEMT6000 || defined KSLIBRARIES_USEALL     // include File in Build only if it ist defined to use it

#include "KSTEMT6000.h"


 
//        float readLightIntensity();
//        float readLightIntensityPercent();
     
 
KSTEMT6000::KSTEMT6000(uint8_t pinSensor) {
    _pinSensor = pinSensor;
    pinMode(_pinSensor, INPUT);
}



int KSTEMT6000::readRawData() {
    return analogRead(_pinSensor);
}



float KSTEMT6000::readIlluminance() {		// return value in lux

    int anaVal = readRawData();

    float volts = anaVal * 5.0 / 4095.0;		// TODO vermutlich passt dieser WErt noch nicht. Wertebereich ist damit 0-1000lux
    float amps = volts / 10000.0;           // across 10.000 Ohms
    float microamps = amps * 1000000;
    float lux = microamps * 2.0;
	return lux;
}



float KSTEMT6000::readLightIntensity() {		// in Percentage
    int anaVal = readRawData();
	float percent = anaVal / 40.95;		// percentage calculation
	return percent;
}


#endif      // #if defined USE_KSTEMT6000 || defined KSLIBRARIES_USEALL