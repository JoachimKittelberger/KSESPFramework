/**
 * @file KSGY302.cpp
 * 
 * @brief implementation for KSGY302 class
 * 
 * @details 
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

#if defined KSGY302 || defined KSLIBRARIES_USEALL     // include File in Build only if it ist defined to use it

#include "KSGY302.h"



KSGY302::KSGY302(uint8_t addrPinLevel, KSCriticalSection* pcsI2C) : _sensor(addrPinLevel) {
	_pcsI2C = pcsI2C;

    // Initialize sensor in modeOneTime, high 0.5 lx resolution
//	init(ModeOneTime, ResolutionHigh);
	init(ModeOneTime, ResolutionMid);
//	init(ModeOneTime, ResolutionLow);
}


void KSGY302::init(BH1750_Mode_e mode, BH1750_Resolution_e resolution) {
	if (_pcsI2C) _pcsI2C->EnterCriticalSection();
   	_sensor.begin(mode, resolution);
	if (_pcsI2C) _pcsI2C->LeaveCriticalSection();
}


int KSGY302::readRawData() {
	int retVal = -1;

   	// Start conversion
	if (_pcsI2C) _pcsI2C->EnterCriticalSection();
    _sensor.startConversion();

    // Wait for completion (blocking busy-wait delay)
    if (_sensor.waitForCompletion()) {
        retVal = _sensor.read();
    }
	if (_pcsI2C) _pcsI2C->LeaveCriticalSection();

	return retVal;
}
 


float KSGY302::readIlluminance() {
	int lux = 0;
	float retVal = NAN;

	lux = readRawData();
	if (lux < 0)
		return retVal;

	// if we have no high resolution-mode we don't need the /2
/*	retVal = lux / 2;
	float decimale = (float)(lux % 10) / 10;						// high rsoluiton -> 0.5 lx
	retVal = retVal + decimale;
*/
	retVal = lux * 1.0;						// mid resolution -> 1.0 lx

    // Print light
//    Serial.print(F("Light: "));
//    Serial.print(lux / 2);
//    Serial.print(F("."));
//    Serial.print(lux % 10);
//    Serial.print(" LUX");
		
//	Serial.printf("   RetVal: %f\n", retVal);
	return retVal;
}



void KSGY302::sleep() {
	if (_pcsI2C) _pcsI2C->EnterCriticalSection();
	_sensor.powerDown();
	if (_pcsI2C) _pcsI2C->LeaveCriticalSection();
}


#endif		// #if defined KSGY302 || defined KSLIBRARIES_USEALL