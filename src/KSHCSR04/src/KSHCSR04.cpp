/**
 * @file KSHCSR04.cpp
 *
 * @brief Implementation file for KSHCSR04 class
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

#if defined USE_KSHCSR04 || defined KSLIBRARIES_USEALL     // include File in Build only if it ist defined to use it



#include "KSHCSR04.h"


KSHCSR04::KSHCSR04(uint8_t pinTrigger, uint8_t pinEcho) {
    _pinTrigger = pinTrigger;
    _pinEcho = pinEcho;
    pinMode(_pinTrigger, OUTPUT);
    pinMode(_pinEcho, INPUT);
}



float KSHCSR04::readDistance() {
     //Using the approximate formula 19.307°C results in roughly 343m/s which is the commonly used value for air.
   return readDistance(19.307);
}



float KSHCSR04::readDistance(float temperature) {
	long duration = 0;

	digitalWrite(_pinTrigger, LOW);
	delayMicroseconds(2);
	digitalWrite(_pinTrigger, HIGH);
	delayMicroseconds(10);
	digitalWrite(_pinTrigger, LOW);

//	duration = pulseIn(echoPin, HIGH);
	duration = pulseIn(_pinEcho, HIGH, 30000);		// ca 30000us Timeout bei 5m. 3000us für 50cm (ist eher bei 3,90m)
	
	// einfache Formel wäre
	//int distance = duration * 0.034 / 2;			// für cm: /29.1/2  für inch: /74/2

	// mit Temperatur-Kompensation:
	float speedofsound = 331.3 + (0.606 * temperature);
	int distance = duration * (speedofsound / 10000) / 2;

    if (distance == 0 || distance > 380) {
        return -1.0 ;
    } else {
        return distance;
    }
}


#endif		// #if defined USE_KSHCSR04 || defined KSLIBRARIES_USEALL