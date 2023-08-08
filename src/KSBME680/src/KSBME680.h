/**
 * @file KSBME680.h
 * 
 * @brief Header for KSBME680 class
 * 
 * @details handles the temperature and humidity module BME680 via I2C
 * 
 * @see
 *   - adafruit/Adafruit BusIO @ ^1.14.1            https://github.com/adafruit/Adafruit_BusIO.git
 *   - adafruit/Adafruit Unified Sensor @ ^1.1.11   https://github.com/adafruit/Adafruit_Sensor.git
 *   - https://github.com/adafruit/Adafruit_BME680.git
 * 
 * @author Joachim Kittelberger <jkittelberger@kibesoft.de>
 * @date 28.07.2023
 * @version 1.00
 *
 * @todo
 *   - Add Class-Headers to all KSLibraries-Files
 *   - Add Function-Headers to all KSLibraries-Files
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
#ifndef _KSBME680_H
#define _KSBME680_H

#include <Adafruit_BME680.h>
#include "KSCriticalSection/src/KSCriticalSection.h"

#define KS_BME_SDA 21
#define KS_BME_SCL 22



#define SEALEVELPRESSURE_HPA (1013.25)

class KSBME680
{
    public:
        KSBME680(uint8_t addr = BME68X_DEFAULT_ADDRESS, TwoWire *theWire = &Wire, KSCriticalSection* pcsI2C = NULL);
        ~KSBME680() {};
  
        void init();

        // unblocking calls
        uint32_t beginReading();
        bool endReading();
        // Temperature (Celsius) assigned after calling performReading() or endReading()
        float getTemperature() { return _bme.temperature; }
        // Pressure (Pascals) assigned after calling performReading() or endReading()
        uint32_t getPressure() { return _bme.pressure; }
        // Humidity (RH %) assigned after calling performReading() or endReading()
        float getHumidity() { return _bme.humidity; }
        //Gas resistor (ohms) assigned after calling performReading() or endReading()
        uint32_t getGas_resistance() { return _bme.gas_resistance; }


    

        // just use blocking mode for Reading
        float readTemperature();
        float readPressure();
        float readHumidity();
        float readAltitude(float seaLevel = SEALEVELPRESSURE_HPA);
        uint32_t readGas();
        bool read(float* pTemperature = NULL, float* pHumidity = NULL, float* pPressure = NULL, float* pAltitude = NULL, uint32_t* pGas = NULL);
    
    
        String getAirQuality();
    protected:
        void GetGasReference();
        String CalculateIAQ(float score);



    private:
        Adafruit_BME680 _bme;
        uint8_t _I2Caddr = BME68X_DEFAULT_ADDRESS;
        TwoWire* _pWire = &Wire;

        KSCriticalSection* _pcsI2C = NULL;
        bool _isInitialized = false;


        float hum_score, gas_score;
        float gas_reference = 250000;
        float hum_reference = 40;
        int   getgasreference_count = 0;


};


#endif  // #define _KSBME680_H