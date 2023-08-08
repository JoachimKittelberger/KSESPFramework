/**
 * @file KSLEDs.cpp
 *
 * @brief Implementation file for KSLEDs class
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

#if defined USE_KSLEDs || defined KSLIBRARIES_USEALL     // include File in Build only if it ist defined to use it

#include "KSLEDs.h"





KSLEDs::KSLEDs(uint8_t pinLEDRed, uint8_t pinLEDGreen, uint8_t pinLEDBlue) {
    _pinLEDRed = pinLEDRed;
    _pinLEDGreen = pinLEDGreen;
    _pinLEDBlue = pinLEDBlue;
}


void KSLEDs::init() {
    pinMode(_pinLEDRed, OUTPUT);
    pinMode(_pinLEDGreen, OUTPUT);
    pinMode(_pinLEDBlue, OUTPUT);
	
	
	// Unlike traditional Arduino, we do not setup our PWM output pins
  	// here with pinMode. Instead, we setup our PWM timer channel and then
  	// we'll attach pins to the PWM channel. The cool side effect here, of course,
  	// is that it's easy to control multiple pins at once with the same PWM timer
  	ledcSetup(PWM_CHANNEL_RED, PWM_FREQ, PWM_RESOLUTION);
  	ledcSetup(PWM_CHANNEL_GREEN, PWM_FREQ, PWM_RESOLUTION);
  	ledcSetup(PWM_CHANNEL_BLUE, PWM_FREQ, PWM_RESOLUTION);
	
  	ledcAttachPin(_pinLEDRed, PWM_CHANNEL_RED);
  	ledcAttachPin(_pinLEDGreen, PWM_CHANNEL_GREEN);
  	ledcAttachPin(_pinLEDBlue, PWM_CHANNEL_BLUE);

	setRedLED(KSLED_OFF);
	setGreenLED(KSLED_OFF);
	setBlueLED(KSLED_OFF);
}

int KSLEDs::getDutyCycleFromPercent(int percent) {
	return (MAX_DUTY_CYCLE * percent / 100);
}


void KSLEDs::setRedLED(bool newState, int brightness) {
	if (newState == KSLED_ON) {
		_brightnessLEDRed = brightness;
		if (_brightnessLEDRed < 0) _brightnessLEDRed = 0;
		if (_brightnessLEDRed > 100) _brightnessLEDRed = 100;

		ledcWrite(PWM_CHANNEL_RED, getDutyCycleFromPercent(_brightnessLEDRed));
	} else {
		ledcWrite(PWM_CHANNEL_RED, 0);
	}
}


void KSLEDs::setGreenLED(bool newState, int brightness) {
	if (newState == KSLED_ON) {
		_brightnessLEDGreen = brightness;
		if (_brightnessLEDGreen < 0) _brightnessLEDGreen = 0;
		if (_brightnessLEDGreen > 100) _brightnessLEDGreen = 100;

		ledcWrite(PWM_CHANNEL_GREEN, getDutyCycleFromPercent(_brightnessLEDGreen));
	} else {
		ledcWrite(PWM_CHANNEL_GREEN, 0);
	}
}


void KSLEDs::setBlueLED(bool newState, int brightness) {
	if (newState == KSLED_ON) {
		_brightnessLEDBlue = brightness;
		if (_brightnessLEDBlue < 0) _brightnessLEDBlue = 0;
		if (_brightnessLEDBlue > 100) _brightnessLEDBlue = 100;

		ledcWrite(PWM_CHANNEL_BLUE, getDutyCycleFromPercent(_brightnessLEDBlue));
	} else {
		ledcWrite(PWM_CHANNEL_BLUE, 0);
	}
}



void KSLEDs::setRGB(bool newState, int brightnessRed, int brightnessGreen, int brightnessBlue) {

	setRedLED(newState, brightnessRed);
	setGreenLED(newState, brightnessGreen);
	setBlueLED(newState, brightnessBlue);

}


#endif		// #if defined USE_KSLEDs || defined KSLIBRARIES_USEALL