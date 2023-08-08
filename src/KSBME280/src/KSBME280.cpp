/**
 * @file KSBME280.cpp
 * 
 * @brief implementation for KSBME280 class
 * 
 * @details handles the temperature and humidity module BME280 via I2C
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

#if defined USE_KSBME280 || defined KSLIBRARIES_USEALL    // include File in Build only if it ist defined to use it


#include "KSBME280.h"


KSBME280::KSBME280(uint8_t addr, TwoWire *theWire, KSCriticalSection* pcsI2C) : _bme(pcsI2C) {
    _I2Caddr = addr;
    _pWire = theWire;
    _pcsI2C = pcsI2C;
}


void KSBME280::init() {
    bool status;

    // default settings
    // (you can also pass in a Wire library object like &Wire2)

//    status = _bme.begin(0x76);  
    if (_pcsI2C) _pcsI2C->EnterCriticalSection();
    status = _bme.begin(_I2Caddr, _pWire);
    _isInitialized = status;
    if (!status) {
        Serial.println("Error: Could not find a valid BME280 sensor, check wiring!");
    } else {
        // set forced mode
        //_bme.setMode(MODE_FORCED);
    }
    if (_pcsI2C) _pcsI2C->LeaveCriticalSection();
}



float KSBME280::readTemperature() {
    float fRetVal = NAN;
    if (_isInitialized) {
        if (_pcsI2C) _pcsI2C->EnterCriticalSection();
        fRetVal = _bme.readTemperature();
        if (_pcsI2C) _pcsI2C->LeaveCriticalSection();
    }
    return fRetVal;
}



float KSBME280::readPressure() {
    float fRetVal = NAN;
    if (_isInitialized) {
        if (_pcsI2C) _pcsI2C->EnterCriticalSection();
        fRetVal = _bme.readPressure() / 100.0;        // This will give the perssure in hPa
        if (_pcsI2C) _pcsI2C->LeaveCriticalSection();
    }
    return fRetVal;
}



float KSBME280::readHumidity() {
    float fRetVal = NAN;
    if (_isInitialized) {
        if (_pcsI2C) _pcsI2C->EnterCriticalSection();
        fRetVal = _bme.readHumidity();              // This will give the humidity in %
        if (_pcsI2C) _pcsI2C->LeaveCriticalSection();
    }
    return fRetVal;
}



float KSBME280::readAltitude(float seaLevel) {
    float fRetVal = NAN;
    if (_isInitialized) {
        if (_pcsI2C) _pcsI2C->EnterCriticalSection();
        fRetVal = _bme.readAltitude(seaLevel);
        if (_pcsI2C) _pcsI2C->LeaveCriticalSection();
    }
    return fRetVal;
}



bool KSBME280::read(float* pTemperature, float* pHumidity, float* pPressure, float* pAltitude) {
    bool bRetVal = true;

    if (!pTemperature && !pPressure && !pHumidity && !pAltitude) return false;

    if (pTemperature) {
        *pTemperature = this->readTemperature();
        if (isnan(*pTemperature)) {
    		Serial.println(F("Failed to read from BME sensor!"));
            bRetVal = false;
        }
    }

    if (pHumidity) {
        *pHumidity = this->readHumidity();
        if (isnan(*pHumidity)) {
    		Serial.println(F("Failed to read from BME sensor!"));
            bRetVal = false;
        }
    }

    if (pPressure) {
        *pPressure = this->readPressure();
        if (isnan(*pPressure)) {
    		Serial.println(F("Failed to read from BME sensor!"));
            bRetVal = false;
        }
    }

    if (pAltitude) {
        *pAltitude = this->readAltitude();
        if (isnan(*pAltitude)) {
    		Serial.println(F("Failed to read from BME sensor!"));
            bRetVal = false;
        }
    }

    return bRetVal;
}




/*



// lese alle Daten auf einmal, da ansonsten _ccs811.available nach dem ersten lesen fehlschlägt und erst nach 2s wieder neue Werte da sind
bool KSBME280::read(int* peCO2, int* pTVOC) {
    bool bRetVal = true;

    if (!peCO2 && !pTVOC) return false;

    _bme.readTemperature();
    _bme.readPressure();
    _bme.readAltitude(SEALEVELPRESSURE_HPA));
    _bme.readHumidity();





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
			Serial.printf("ERROR!  %d Reading CCS811\n", retVal);
            bRetVal = false;
		}
	}
	else {
		Serial.println(F("WARNING! CCS811 new data not available"));
	}
    return bRetVal;
}

*/


#endif      // #if defined USE_KSBME280 || defined KSLIBRARIES_USEALL