/**
 * @file KSCCS811.cpp
 * 
 * @brief Implementation for KSCCS811 class
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


#if defined USE_KSCCS811 || defined KSLIBRARIES_USEALL    // include File in Build only if it ist defined to use it


#include "KSCCS811.h"


KSCCS811::KSCCS811(KSCriticalSection* pcsI2C) {
    _pcsI2C = pcsI2C;
}


void KSCCS811::init() {
    if (_pcsI2C) _pcsI2C->EnterCriticalSection();

	if (!_ccs811.begin()) {
		Serial.println(F("[KSCCS811] Failed to start CCS811 sensor!"));
		// TODO: hier eine Error-LED angehen lassen? Oder in Thread nochmals versuchen, bis es klappt
	}
	while (!_ccs811.available());	// Wait for the sensor to be ready

	_ccs811.setDriveMode(CCS811_DRIVE_MODE_1SEC);

    if (_pcsI2C) _pcsI2C->LeaveCriticalSection();
}



int KSCCS811::readeCO2() {
    int retVal = -1;

    if (_pcsI2C) _pcsI2C->EnterCriticalSection();

	if (_ccs811.available()) {
		retVal = _ccs811.readData();
		if (retVal == 0) {
			retVal = _ccs811.geteCO2();
		}
		else {
			Serial.printf("[KSCCS811] ERROR!  %d Reading CCS811\n", retVal);
            retVal = -1;
		}
	}
	else {
		Serial.println(F("[KSCCS811] WARNING! CCS811 new data not available"));
	}
    if (_pcsI2C) _pcsI2C->LeaveCriticalSection();
    return retVal;
}



int KSCCS811::readTVOC() {
    int retVal = -1;

    if (_pcsI2C) _pcsI2C->EnterCriticalSection();
	if (_ccs811.available()) {
		retVal = _ccs811.readData();
		if (retVal == 0) {
			retVal = _ccs811.getTVOC();
		}
		else {
			Serial.printf("[KSCCS811] ERROR!  %d Reading CCS811\n", retVal);
            retVal = -1;
		}
	}
	else {
		Serial.println(F("[KSCCS811] WARNING! CCS811 new data not available"));
	}
    if (_pcsI2C) _pcsI2C->LeaveCriticalSection();
    return retVal;

}


// lese alle Daten auf einmal, da ansonsten _ccs811.available nach dem ersten lesen fehlschlägt und erst nach 2s wieder neue Werte da sind
bool KSCCS811::read(int* peCO2, int* pTVOC) {
    bool bRetVal = true;

    if (!peCO2 && !pTVOC) return false;

    if (_pcsI2C) _pcsI2C->EnterCriticalSection();
	if (_ccs811.available()) {
		int retVal = _ccs811.readData();
		if (retVal == 0) {
            if (peCO2) {
                *peCO2 = _ccs811.geteCO2();
            }
            if (pTVOC) {
                *pTVOC = _ccs811.getTVOC();
            }
		}
		else {
			Serial.printf("[KSCCS811] ERROR!  %d Reading CCS811\n", retVal);
            bRetVal = false;
		}
	}
	else {
		Serial.println(F("[KSCCS811] WARNING! CCS811 new data not available"));
	}
    if (_pcsI2C) _pcsI2C->LeaveCriticalSection();
    return bRetVal;
}


#endif		// #if defined USE_KSCCS811 || defined KSLIBRARIES_USEALL