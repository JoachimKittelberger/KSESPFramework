/**
 * @file KSMQ135.cpp
 * 
 * @brief implementation file for using KSMQ135 class
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

#if defined USE_KSMQ135 || defined KSLIBRARIES_USEALL     // include File in Build only if it ist defined to use it

#include "KSMQ135.h"

#include "KSLogger/src/KSLogger.h"


KSMQ135::KSMQ135(uint8_t anaPin) : _mq135(anaPin) {
    _anaPin = anaPin;
}


void KSMQ135::init() {
	pinMode(_anaPin, INPUT);
	
	// TODO: die nachfolgenden Zeilen sollten nochmals überprüft werden
//	adcAttachPin(_anaPin);
//	analogReadResolution(ADC_Bit_Resolution);		// default
//	analogSetPinAttenuation(_anaPin, ADC_11db);
}


float KSMQ135::readCO2() {
	int val = analogRead(_anaPin);
//	LOGGER.printf("MQ135: raw: %d", val);
	float zero = _mq135.getRZero();
//	LOGGER.printf("  RZero: %.2f", zero);
	float ppm = _mq135.getPPM();
//	LOGGER.printf("  ppm: %.2f\n", ppm);

    return ppm;
}


bool KSMQ135::read(float* pCO2) {
    bool bRetVal = true;

	if (!pCO2) return false;

	if (pCO2) {
		*pCO2 = this->readCO2();
		if (isnan(*pCO2)) bRetVal = false;
	}

	//	float volt = MQ135.getVoltage();
	//	LOGGER.printf("MQ135.getVoltage() = %.02f\n", volt);

/*
	double anaval = analogRead(_anaPin); // reference Voltage ist 3v3 so maximum reading is 3v3 in range 0 to 4095
	
	// convert to exact voltage. Genauigkeit vorher 7% jetzt < 1% (Polynominal equation)
	double volt = 0.0;
	if (anaval < 1) {
		volt = 0.0;
	} else if (anaval >= 4095) {
		volt = 3.3;
	} else {
		volt = -0.000000000000016 * pow(anaval, 4) + 0.000000000118171 * pow(anaval, 3) - 0.000000301211691 * pow(anaval, 2) + 0.001109019271794 * anaval + 0.034143524634089;
	}
	LOGGER.printf("Analog-Value Sensor: %.2f Volt: %.2f\n", anaval, volt);
*/

    return bRetVal;
}



#endif		// #if defined USE_KSMQ135 || defined KSLIBRARIES_USEALL