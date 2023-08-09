/**
 * @file KSMHZ19b.cpp
 * 
 * @brief Implementation file for using KSMHZ19b class
 * 
 * @details 
 * 
 * @see
 * 
 * @author Joachim Kittelberger <jkittelberger@kibesoft.de>
 * @date 20.06.2022
 * @version 1.00
 *
 * @todo
 *
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

#if defined USE_KSMHZ19b || defined KSLIBRARIES_USEALL     // include File in Build only if it ist defined to use it

#include "KSMHZ19b.h"

#include "KSLogger/src/KSLogger.h"



KSMHZ19b::KSMHZ19b(int8_t RXPin, uint8_t TXPin) {
    _RXPin = RXPin;
    _TXPin = TXPin;



}


void KSMHZ19b::init() {
    _serial.begin(MHZ19B_BAUDRATE, SWSERIAL_8N1, _RXPin, _TXPin); // (ESP32 Example) device to MH-Z19 serial start   
    _mhz19b.begin(_serial);                                      // *Serial(Stream) refence must be passed to library begin(). 
    _mhz19b.autoCalibration(true);                              // Turn auto calibration ON (OFF autoCalibration(false))
}



int KSMHZ19b::readCO2() {
    /* note: getCO2() default is command "CO2 Unlimited". This returns the correct CO2 reading even 
      if below background CO2 levels or above range (useful to validate sensor). You can use the 
      usual documented command with getCO2(false) */
	int CO2 = _mhz19b.getCO2(true, true);                             // Request CO2 (as ppm)
	if (_mhz19b.errorCode != RESULT_OK) {
		LOGGER.print(F("Failed to read from MHZ19 sensor! Error: "));
		LOGGER.println(_mhz19b.errorCode);
        CO2 = -1;
	}
    return CO2;
}


bool KSMHZ19b::read(int* pCO2) {
    bool bRetVal = true;

    if (!pCO2) return false;

    if (pCO2) {
        *pCO2 = readCO2();
        if (_mhz19b.errorCode != RESULT_OK) {
            bRetVal = false;
        }
    }
    return bRetVal;
}


#endif      // #if defined USE_KSMHZ19b || defined KSLIBRARIES_USEALL