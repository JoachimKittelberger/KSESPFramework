/**
 * @file KSButton.cpp
 * 
 * @brief Implementation for KSButton class
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

#if defined USE_KSButton || defined KSLIBRARIES_USEALL    // include File in Build only if it ist defined to use it

#include "KSButton.h"




KSButton::KSButton(uint8_t pin) {
    _pin = pin;
}



void KSButton::init() {
	// set up button Pin
  	pinMode (_pin, INPUT);
  	pinMode(_pin, INPUT_PULLUP);  // Pull up to 3.3V on input - some buttons already have this done

//  	attachInterrupt(digitalPinToInterrupt(_pin), handleButtonInterrupt, FALLING);

}


bool KSButton::isPressed() {

	int pinState = digitalRead(_pin);
	if (pinState == LOW) {
  		vTaskDelay(pdMS_TO_TICKS(10));
		if (pinState == LOW) return true;
	}
	return false;
}




/*

//
//
// Debounced button task
//
//

volatile int numberOfButtonInterrupts = 0;
volatile bool lastState;
volatile uint32_t debounceTimeout = 0;

// For setting up critical sections (enableinterrupts and disableinterrupts not available)
// used to disable and interrupt interrupts

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;


// Interrupt Service Routine - Keep it short!
void IRAM_ATTR handleButtonInterrupt() {
  portENTER_CRITICAL_ISR(&mux);
  numberOfButtonInterrupts++;
  lastState = digitalRead(_pin);
  debounceTimeout = xTaskGetTickCount(); //version of millis() that works from interrupt
  portEXIT_CRITICAL_ISR(&mux);
}

//
// RTOS Task for reading button pushes (debounced)
//

void taskButtonRead( void * parameter)
{
	String taskMessage = "Debounced ButtonRead Task running on core ";
	taskMessage = taskMessage + xPortGetCoreID();
	Serial.println(taskMessage);

	// set up button Pin
	pinMode (_pin, INPUT);
	pinMode(_pin, INPUT_PULLUP);  // Pull up to 3.3V on input - some buttons already have this done

	attachInterrupt(digitalPinToInterrupt(_pin), handleButtonInterrupt, FALLING);

	uint32_t saveDebounceTimeout;
	bool saveLastState;
	int save;

	// Enter RTOS Task Loop
	while (1) {

		portENTER_CRITICAL_ISR(&mux); // so that value of numberOfButtonInterrupts,l astState are atomic - Critical Section
		save  = numberOfButtonInterrupts;
		saveDebounceTimeout = debounceTimeout;
		saveLastState  = lastState;
		portEXIT_CRITICAL_ISR(&mux); // end of Critical Section

		bool currentState = digitalRead(BUTTONPIN);

		// This is the critical IF statement
		// if Interrupt Has triggered AND Button Pin is in same state AND the debounce time has expired THEN you have the button push!
		//
		if ((save != 0) //interrupt has triggered
			&& (currentState == saveLastState) // pin is still in the same state as when intr triggered
			&& (millis() - saveDebounceTimeout > DEBOUNCETIME ))
		{ // and it has been low for at least DEBOUNCETIME, then valid keypress
		
		if (currentState == LOW)
		{
			Serial.printf("Button is pressed and debounced, current tick=%d\n", millis());
		}
		else
		{
			Serial.printf("Button is released and debounced, current tick=%d\n", millis());
		}
		
		Serial.printf("Button Interrupt Triggered %d times, current State=%u, time since last trigger %dms\n", save, currentState, millis() - saveDebounceTimeout);
		
		portENTER_CRITICAL_ISR(&mux); // can't change it unless, atomic - Critical section
		numberOfButtonInterrupts = 0; // acknowledge keypress and reset interrupt counter
		portEXIT_CRITICAL_ISR(&mux);


		vTaskDelay(10 / portTICK_PERIOD_MS);
		}

		vTaskDelay(10 / portTICK_PERIOD_MS);

	}
	
}





// alternativ mit Lambda-Funktion

volatile unsigned long DebounceTimer;
volatile int ButtonPressed;
volatile unsigned int delayTime = 100;
#define pin 3
void setup() {
  pinMode(pin, INPUT_PULLUP);
  Serial.begin(115200);
  Serial.println(F("Start: Light Weight depounce blink without delay code"));
  attachInterrupt(digitalPinToInterrupt(pin), [] {if (ButtonPressed+= (millis() - DebounceTimer) >= (delayTime )) DebounceTimer = millis();}, RISING);
  //attachInterrupt(digitalPinToInterrupt(pin), []{ButtonPressed++;}, RISING); // No Debounce
}
void loop() {
  if (ButtonPressed> 0) Serial.println(Pressed ); 
  ButtonPressed = 0; // Must clear
}


*/
#endif      // #if defined USE_KSButton || defined KSLIBRARIES_USEALL