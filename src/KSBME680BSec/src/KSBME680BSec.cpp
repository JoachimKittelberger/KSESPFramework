/**
 * @file KSBME680BSec.cpp
 * 
 * @brief Implementation for KSBME680BSec class
 * 
 * @details handles the temperature and humidity module BME680 via I2C
 *   This is a library for the BME680 gas, humidity, temperature & pressure sensor
 *   Designed specifically to work with the Adafruit BME680 Breakout
 *   ----> http://www.adafruit.com/products/3660
 *   These sensors use I2C or SPI to communicate, 2 or 4 pins are required
 *   to interface.
 *   Adafruit invests time and resources providing this open source code,
 *   please support Adafruit and open-source hardware by purchasing products
 *   from Adafruit!
 *   Written by Limor Fried & Kevin Townsend for Adafruit Industries.
 *   BSD license, all text above must be included in any redistribution
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

#if defined USE_KSBME680BSec || defined KSLIBRARIES_USEALL    // include File in Build only if it ist defined to use it

#include "KSBME680BSec.h"


KSCriticalSection* KSBsec::_pcsI2C = NULL;
uint8_t KSBsec::_I2Caddr = BME68X_I2C_ADDR_HIGH;




KSBME680BSec::KSBME680BSec(uint8_t addr, TwoWire *theWire, KSCriticalSection* pcsI2C) : sensorList {
    BSEC_OUTPUT_RAW_TEMPERATURE,
    BSEC_OUTPUT_RAW_PRESSURE,
    BSEC_OUTPUT_RAW_HUMIDITY,
    BSEC_OUTPUT_RAW_GAS,
    BSEC_OUTPUT_IAQ,
    BSEC_OUTPUT_STATIC_IAQ,
    BSEC_OUTPUT_CO2_EQUIVALENT,
    BSEC_OUTPUT_BREATH_VOC_EQUIVALENT,
    BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_TEMPERATURE,
    BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_HUMIDITY }, iaqSensor(pcsI2C)
 {
    _I2Caddr = addr;
    _pWire = theWire;
    _pcsI2C = pcsI2C;
}


bool KSBME680BSec::init() {
    bool status = false;

    iaqSensor.begin(_I2Caddr, *_pWire);
    status = checkIaqSensorStatus();
    _isInitialized = status;
    
    if (!status) {
        Serial.println("Error: Could not find a valid BME680 sensor, check wiring!");
    } else {
        String output = "\nBME680 BSEC library version " + String(iaqSensor.version.major) + "." + String(iaqSensor.version.minor) + "." + String(iaqSensor.version.major_bugfix) + "." + String(iaqSensor.version.minor_bugfix);
        Serial.println(output);

        //iaqSensor.updateSubscription(sensorList, sizeof(sensorList) / sizeof(sensorList[0]), BSEC_SAMPLE_RATE_LP);
        iaqSensor.updateSubscription(sensorList, 10, BSEC_SAMPLE_RATE_LP);
        status = checkIaqSensorStatus();
    }
    return status;
}




// Helper function definitions
bool KSBME680BSec::checkIaqSensorStatus()
{
    bool bRetVal = true;
    String output = "";
    if (iaqSensor.bsecStatus != BSEC_OK) {
        if (iaqSensor.bsecStatus < BSEC_OK) {
            output = "BSEC error code : " + String(iaqSensor.bsecStatus);
            bRetVal = false;
            //for (;;)  delay(10);
        } else {
            output = "BSEC warning code : " + String(iaqSensor.bsecStatus);
        }
        Serial.println(output);
    }

    if (iaqSensor.bme68xStatus != BME68X_OK) {
        if (iaqSensor.bme68xStatus < BME68X_OK) {
            output = "BME68x error code : " + String(iaqSensor.bme68xStatus);
            bRetVal = false;
            //for (;;)  delay(10);
        } else {
            output = "BME68x warning code : " + String(iaqSensor.bme68xStatus);
        }
        Serial.println(output);
    }
    return bRetVal;
}






/*


uint32_t KSBME680BSec::beginReading() {
    uint32_t uRetVal = 0;
    if (_isInitialized) {
        if (_pcsI2C) _pcsI2C->EnterCriticalSection();
        uRetVal = _bme.beginReading();
        if (_pcsI2C) _pcsI2C->LeaveCriticalSection();
    }
    return uRetVal;
}



bool KSBME680BSec::endReading() {
    bool bRetVal = false;
    if (_isInitialized) {
        if (_pcsI2C) _pcsI2C->EnterCriticalSection();
        bRetVal = _bme.endReading();
        if (_pcsI2C) _pcsI2C->LeaveCriticalSection();
    }
    return bRetVal;
}





float KSBME680BSec::readTemperature() {
    float fRetVal = NAN;
    if (_isInitialized) {
        if (_pcsI2C) _pcsI2C->EnterCriticalSection();
        fRetVal = _bme.readTemperature();
        if (_pcsI2C) _pcsI2C->LeaveCriticalSection();
    }
    return fRetVal;
}



float KSBME680BSec::readPressure() {
    float fRetVal = NAN;
    if (_isInitialized) {
        if (_pcsI2C) _pcsI2C->EnterCriticalSection();
        fRetVal = _bme.readPressure() / 100.0;        // This will give the perssure in hPa
        if (_pcsI2C) _pcsI2C->LeaveCriticalSection();
    }
    return fRetVal;
}



float KSBME680BSec::readHumidity() {
    float fRetVal = NAN;
    if (_isInitialized) {
        if (_pcsI2C) _pcsI2C->EnterCriticalSection();
        fRetVal = _bme.readHumidity();              // This will give the humidity in %
        if (_pcsI2C) _pcsI2C->LeaveCriticalSection();
    }
    return fRetVal;
}



float KSBME680BSec::readAltitude(float seaLevel) {
    float fRetVal = NAN;
    if (_isInitialized) {
        if (_pcsI2C) _pcsI2C->EnterCriticalSection();
        fRetVal = _bme.readAltitude(seaLevel);
        if (_pcsI2C) _pcsI2C->LeaveCriticalSection();
    }
    return fRetVal;
}



uint32_t KSBME680BSec::readGas() {
    uint32_t uRetVal = 0;
    if (_isInitialized) {
        if (_pcsI2C) _pcsI2C->EnterCriticalSection();
        uRetVal = _bme.readGas();
        if (_pcsI2C) _pcsI2C->LeaveCriticalSection();
    }
    return uRetVal;
}


bool KSBME680BSec::read(float* pTemperature, float* pHumidity, float* pPressure, float* pAltitude, uint32_t* pGas) {
    bool bRetVal = true;

    if (!pTemperature && !pPressure && !pHumidity && !pAltitude && !pGas) return false;

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

    if (pGas) {
        *pGas = this->readGas();
        if ((*pGas) == 0) {
    		Serial.println(F("Failed to read from BME sensor!"));
            bRetVal = false;
        }
    }

    return bRetVal;
}


*/

/*



// lese alle Daten auf einmal, da ansonsten _ccs811.available nach dem ersten lesen fehlschlägt und erst nach 2s wieder neue Werte da sind
bool KSBME680::read(int* peCO2, int* pTVOC) {
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


#endif      // #if defined USE_KSBME680BSec || defined KSLIBRARIES_USEALL