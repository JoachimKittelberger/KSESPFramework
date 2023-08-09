/**
 * @file KSBME680BSec.h
 * 
 * @brief Header for KSBME680BSec class
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
 *   - Library unter https://github.com/boschsensortec/BSEC-Arduino-library.git
 *   - BME680 Breakout: http://www.adafruit.com/products/3660
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

#ifndef _KSBME680BSEC_H
#define _KSBME680BSEC_H


#include "bsec.h"

#include "KSCriticalSection/src/KSCriticalSection.h"

#define KS_BME_SDA 21
#define KS_BME_SCL 22



#define SEALEVELPRESSURE_HPA (1013.25)

// save i2c access with critical section
class KSBsec : public Bsec {
    public:

        KSBsec(KSCriticalSection* pcsI2C = NULL) : Bsec() {
            _pcsI2C = pcsI2C;
        }

        void begin(uint8_t i2cAddr, TwoWire &i2c, bme68x_delay_us_fptr_t idleTask = delay_us) {
            _I2Caddr = i2cAddr;
            Bsec::wireObj = &i2c;
//            Bsec::begin(i2cAddr, BME680_I2C_INTF, i2cRead, i2cWrite, idleTask);
            Bsec::begin(BME68X_I2C_INTF, i2cRead, i2cWrite, idleTask, (void*)(intptr_t)_I2Caddr);
        }

        bool sleep() {
/*
            // TODO !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            _bme680.power_mode = BME680_SLEEP_MODE;

            if (bme680_set_sensor_mode(&_bme680) != BME680_OK) {
                LOGGER.println("[KSBsec]: Error bme680_set_sensor_mode()");
                return false;
            }
*/
            return true;
        }

    private:    
 	    static int8_t i2cRead(uint8_t regAddr, uint8_t *regData, uint32_t length, void *intfPtr) {
 //       static int8_t i2cRead(uint8_t devId, uint8_t regAddr, uint8_t *regData, uint16_t length) {
            //LOGGER.println("i2cRead");
            uint8_t retVal = 0;
            if (_pcsI2C) _pcsI2C->EnterCriticalSection();
            retVal = Bsec::i2cRead(regAddr, regData, length, (void*)(intptr_t)_I2Caddr);
            if (_pcsI2C) _pcsI2C->LeaveCriticalSection();
            return retVal;
        }

    
 	    static int8_t i2cWrite(uint8_t regAddr, const uint8_t *regData, uint32_t length, void *intfPtr) {
//        static int8_t i2cWrite(uint8_t devId, uint8_t regAddr, uint8_t *regData, uint16_t length) {
            //LOGGER.println("i2cWrite");
            uint8_t retVal = 0;
            if (_pcsI2C) _pcsI2C->EnterCriticalSection();
            retVal = Bsec::i2cWrite(regAddr, regData, length, (void*)(intptr_t)_I2Caddr);
            if (_pcsI2C) _pcsI2C->LeaveCriticalSection();
            return retVal;
        }

    private:
        static KSCriticalSection* _pcsI2C;
        static uint8_t _I2Caddr;

};



class KSBME680BSec
{
    public:
        KSBME680BSec(uint8_t addr = BME68X_I2C_ADDR_HIGH, TwoWire *theWire = &Wire, KSCriticalSection* pcsI2C = NULL);
        ~KSBME680BSec() {};
  
        bool init();

       /* 
        
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
*/
        bsec_virtual_sensor_t sensorList[10];

        bool sleep() {
            return iaqSensor.sleep();
        }

    //protected:
        bool checkIaqSensorStatus();


    public:
        //Bsec iaqSensor;
        KSBsec iaqSensor;

    protected:
        uint8_t _I2Caddr = BME68X_I2C_ADDR_HIGH;
        TwoWire* _pWire = &Wire;

        KSCriticalSection* _pcsI2C = NULL;
        bool _isInitialized = false;
};


#endif  // #define _KSBME680BSEC_H