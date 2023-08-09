/**
 * @file KSBME680.cpp
 * 
 * @brief implementation for KSBME680 class
 * 
 * @details handles the temperature and humidity module BME680 via I2C
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

#if defined USE_KSBME680 || defined KSLIBRARIES_USEALL    // include File in Build only if it ist defined to use it


#include "KSBME680.h"

#include "KSLogger/src/KSLogger.h"


KSBME680::KSBME680(uint8_t addr, TwoWire *theWire, KSCriticalSection* pcsI2C) {
    _I2Caddr = addr;
    _pWire = theWire;
    _pcsI2C = pcsI2C;
}


void KSBME680::init() {
    bool status;

    // default settings
    // (you can also pass in a Wire library object like &Wire2)

//    status = _bme.begin(0x76);  
    if (_pcsI2C) _pcsI2C->EnterCriticalSection();
    status = _bme.begin(_I2Caddr, _pWire);
    _isInitialized = status;
    if (!status) {
        LOGGER.println("Error: Could not find a valid BME680 sensor, check wiring!");
    } else {
        // Set up oversampling and filter initialization
        _bme.setTemperatureOversampling(BME680_OS_8X);
        _bme.setHumidityOversampling(BME680_OS_2X);
        _bme.setPressureOversampling(BME680_OS_4X);
        _bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
        _bme.setGasHeater(320, 150); // 320*C for 150 ms
    }
    if (_pcsI2C) _pcsI2C->LeaveCriticalSection();
}



uint32_t KSBME680::beginReading() {
    uint32_t uRetVal = 0;
    if (_isInitialized) {
        if (_pcsI2C) _pcsI2C->EnterCriticalSection();
        uRetVal = _bme.beginReading();
        if (_pcsI2C) _pcsI2C->LeaveCriticalSection();
    }
    return uRetVal;
}



bool KSBME680::endReading() {
    bool bRetVal = false;
    if (_isInitialized) {
        if (_pcsI2C) _pcsI2C->EnterCriticalSection();
        bRetVal = _bme.endReading();
        if (_pcsI2C) _pcsI2C->LeaveCriticalSection();
    }
    return bRetVal;
}





float KSBME680::readTemperature() {
    float fRetVal = NAN;
    if (_isInitialized) {
        if (_pcsI2C) _pcsI2C->EnterCriticalSection();
        fRetVal = _bme.readTemperature();
        if (_pcsI2C) _pcsI2C->LeaveCriticalSection();
    }
    return fRetVal;
}



float KSBME680::readPressure() {
    float fRetVal = NAN;
    if (_isInitialized) {
        if (_pcsI2C) _pcsI2C->EnterCriticalSection();
        fRetVal = _bme.readPressure() / 100.0;        // This will give the perssure in hPa
        if (_pcsI2C) _pcsI2C->LeaveCriticalSection();
    }
    return fRetVal;
}



float KSBME680::readHumidity() {
    float fRetVal = NAN;
    if (_isInitialized) {
        if (_pcsI2C) _pcsI2C->EnterCriticalSection();
        fRetVal = _bme.readHumidity();              // This will give the humidity in %
        if (_pcsI2C) _pcsI2C->LeaveCriticalSection();
    }
    return fRetVal;
}



float KSBME680::readAltitude(float seaLevel) {
    float fRetVal = NAN;
    if (_isInitialized) {
        if (_pcsI2C) _pcsI2C->EnterCriticalSection();
        fRetVal = _bme.readAltitude(seaLevel);
        if (_pcsI2C) _pcsI2C->LeaveCriticalSection();
    }
    return fRetVal;
}



uint32_t KSBME680::readGas() {
    uint32_t uRetVal = 0;
    if (_isInitialized) {
        if (_pcsI2C) _pcsI2C->EnterCriticalSection();
        uRetVal = _bme.readGas();
        if (_pcsI2C) _pcsI2C->LeaveCriticalSection();
    }
    return uRetVal;
}


bool KSBME680::read(float* pTemperature, float* pHumidity, float* pPressure, float* pAltitude, uint32_t* pGas) {
    bool bRetVal = true;

    if (!pTemperature && !pPressure && !pHumidity && !pAltitude && !pGas) return false;

    if (pTemperature) {
        *pTemperature = this->readTemperature();
        if (isnan(*pTemperature)) {
    		LOGGER.println(F("Failed to read from BME sensor!"));
            bRetVal = false;
        }
    }

    if (pHumidity) {
        *pHumidity = this->readHumidity();
        if (isnan(*pHumidity)) {
    		LOGGER.println(F("Failed to read from BME sensor!"));
            bRetVal = false;
        }
    }

    if (pPressure) {
        *pPressure = this->readPressure();
        if (isnan(*pPressure)) {
    		LOGGER.println(F("Failed to read from BME sensor!"));
            bRetVal = false;
        }
    }

    if (pAltitude) {
        *pAltitude = this->readAltitude();
        if (isnan(*pAltitude)) {
    		LOGGER.println(F("Failed to read from BME sensor!"));
            bRetVal = false;
        }
    }

    if (pGas) {
        *pGas = this->readGas();
        if ((*pGas) == 0) {
    		LOGGER.println(F("Failed to read from BME sensor!"));
            bRetVal = false;
        }
    }

    return bRetVal;
}




String KSBME680::getAirQuality() {
    float current_humidity = readHumidity();
    if (current_humidity >= 38 && current_humidity <= 42)
        hum_score = 0.25*100; // Humidity +/-5% around optimum 
    else
    { //sub-optimal
        if (current_humidity < 38) 
            hum_score = 0.25/hum_reference*current_humidity*100;
        else
        {
            hum_score = ((-0.25/(100-hum_reference)*current_humidity)+0.416666)*100;
        }
    }
    
    //Calculate gas contribution to IAQ index
    float gas_lower_limit = 5000;   // Bad air quality limit
    float gas_upper_limit = 50000;  // Good air quality limit 
    if (gas_reference > gas_upper_limit) gas_reference = gas_upper_limit; 
    if (gas_reference < gas_lower_limit) gas_reference = gas_lower_limit;
    gas_score = (0.75/(gas_upper_limit-gas_lower_limit)*gas_reference -(gas_lower_limit*(0.75/(gas_upper_limit-gas_lower_limit))))*100;
    float air_quality_score = hum_score + gas_score;
    if ((getgasreference_count++)%10==0) GetGasReference(); 
    return CalculateIAQ(air_quality_score);
}


void KSBME680::GetGasReference(){
    int readings = 10;
    for (int i = 1; i <= readings; i++){
        gas_reference += readGas();
    }
    gas_reference = gas_reference / readings;
}


String KSBME680::CalculateIAQ(float score) {
    String IAQ_text = "Air quality is ";
    score = (100 - score) * 5;
    if      (score >= 301)                  IAQ_text += "Hazardous";
    else if (score >= 201 && score <= 300 ) IAQ_text += "Very Unhealthy";
    else if (score >= 176 && score <= 200 ) IAQ_text += "Unhealthy";
    else if (score >= 151 && score <= 175 ) IAQ_text += "Unhealthy for Sensitive Groups";
    else if (score >=  51 && score <= 150 ) IAQ_text += "Moderate";
    else if (score >=  00 && score <=  50 ) IAQ_text += "Good";
    return IAQ_text;
}




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
			LOGGER.printf("ERROR!  %d Reading CCS811\n", retVal);
            bRetVal = false;
		}
	}
	else {
		LOGGER.println(F("WARNING! CCS811 new data not available"));
	}
    return bRetVal;
}

*/


#endif      // #if defined USE_KSBME680 || defined KSLIBRARIES_USEALL