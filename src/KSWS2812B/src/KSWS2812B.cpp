/**
 * @file KSWS2812B.cpp
 * 
 * @brief implementation file for using WS2812B
 * 
 * @details 
 * 
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

#if defined USE_KSWS2812B || defined KSLIBRARIES_USEALL     // include File in Build only if it ist defined to use it

#include "KSWS2812B.h"
#include "KSLogger/src/KSLogger.h"



const LEStruct lightEffects[LightEffect::LENumOfLightEffects] = {
    { LightEffect::LEBlack, "Black", "Set all LEDs to black color", 20 },
    { LightEffect::LEWhite, "White", "Set all LEDs to white color", 20 },
    { LightEffect::LERed, "Red", "Set all LEDs to red color", 20 },
    { LightEffect::LEGreen, "Green", "Set all LEDs to green color", 20 },
    { LightEffect::LEBlue, "Blue", "Set all LEDs to blue color", 20 },
    { LightEffect::LELastColor, "LastColor", "Set all LEDs to last color", 20 },
    { LightEffect::LERainbow, "Rainbow", "Set all LEDs to a static rainbow", 20 },
    { LightEffect::LERainbowCycle, "RainbowCycle", "Set all LEDs to a cylcic rainbow", 500 }    
    // TODO: add here new LightEffects
};

//zugriff über String name = String(lightEffects[curEffect].pEffectName)



KSWS2812B::KSWS2812B(int numPixel, int pin) : _neoPixel(numPixel, pin, NEO_GRB + NEO_KHZ800) {
    _numPixel = numPixel;
    _pin = pin;
}


  
TaskHandle_t KSWS2812B::create() {
	int coreID = xPortGetCoreID();
	UBaseType_t setupPriority = uxTaskPriorityGet(NULL);

	xTaskCreatePinnedToCore(
    	[](void* context){ static_cast<KSWS2812B*>(context)->tKSWS2812B(); },
		"tKSWS2812B",
		8192,
		this,
		setupPriority,
		&_htKSWS2812B,
		coreID
		);

	return _htKSWS2812B;
}
        


void KSWS2812B::tKSWS2812B() {
	Serial.println("created Task tKSWS2812B");
    init();

	for (;;) {
        handle();

		vTaskDelay(pdMS_TO_TICKS(_currentScheduleTimer));
	}	
}



bool KSWS2812B::init() {
    csPixelsAccess.EnterCriticalSection();
    _neoPixel.begin(); // This initializes the NeoPixel library.
    _neoPixel.clear(); // Set all pixel colors to 'off'
    _neoPixel.show();
    csPixelsAccess.LeaveCriticalSection();
    _lightsOff = true;

    return true;
}



// handle the current LightEffect      
void KSWS2812B::handle() {
    if (_lightsOff) {
        setLightsOff();           // sollte eigentlich nicht notwendig sein.
        return;
    }
    if (!_lightEffectChanged) return;

    // calculate the cycles for Animation
    int cyclesUntilNewAnimation = 1;            // animation cycles dependend on _currentAnimationTimeMS
    if (_currentAnimationTimeMS >= _currentScheduleTimer ) {
        cyclesUntilNewAnimation = _currentAnimationTimeMS / _currentScheduleTimer;
    }
    int currentCycle = (_currentAnimationCycle / cyclesUntilNewAnimation) % _numPixel;          // range from 0 to _numPixel - 1
    bool changeAnimation = ((_currentAnimationCycle % cyclesUntilNewAnimation) == 0);           // bool, if animation should change to next step
    //LOGGER.printf("_currentAnimationCycle: %d, cyclesUntilNewAnimation: %d, currentCycle: %d, changeAnimation: %s\n", _currentAnimationCycle, cyclesUntilNewAnimation, currentCycle, changeAnimation ? "true" : "false");

    switch (_currentLightEffect)
    {
    case LEBlack:
        setAllPixelToColor(0, 0, 0);
        _lightEffectChanged = false;        // now we have the new light-Effect
        break;
    
    case LEWhite:
        setAllPixelToColor(255, 255, 255);
        _lightEffectChanged = false;        // now we have the new light-Effect
        break;
 
    case LERed:
        setAllPixelToColor(255, 0, 0);
        _lightEffectChanged = false;        // now we have the new light-Effect
        break;
 
    case LEGreen:
        setAllPixelToColor(0, 255, 0);
        _lightEffectChanged = false;        // now we have the new light-Effect
        break;
 
    case LEBlue:
        setAllPixelToColor(0, 0, 255);
        _lightEffectChanged = false;        // now we have the new light-Effect
        break;
 
    case LELastColor:
        setAllPixelToColor(_lastColor);
        _lightEffectChanged = false;        // now we have the new light-Effect
        break;
 
    case LERainbow:
        csPixelsAccess.EnterCriticalSection();
        _neoPixel.rainbow(0, 1, 255, _currentBrightness, true);
        _neoPixel.show();   // Send the updated pixel colors to the hardware.   
        csPixelsAccess.LeaveCriticalSection();
        _lightEffectChanged = false;        // now we have the new light-Effect
        break;

    case LERainbowCycle:            // Regenbogen lauflicht. 1 Regenbogen über alle LEDs die in der Animationszeit durchlaufen
        // wenn neue Animation stattfinden soll:
        if (changeAnimation) {
            setRainbow(currentCycle, 0, 1, 255, _currentBrightness, true);
        }
        //_lightEffectChanged = true;        // we need new light-Effect for next iteration
        _currentAnimationCycle++;
        break;

    // TODO: add here new LightEffexts

    default:
        String effectName = "";
        if (_currentLightEffect < LightEffect::LENumOfLightEffects)
            effectName = String(lightEffects[_currentLightEffect].pEffectName);
        LOGGER.printf("Error: Unknown LightEffect: %d (%s))\n", _currentLightEffect, effectName);
        setAllPixelToColor(128, 128, 128);
        _lightEffectChanged = false;        // now we have the new light-Effect
        _currentAnimationCycle = 0;
        break;
    }

    // Überlauf bei _currentAnimationCylcle handeln
    if (_currentAnimationCycle > (cyclesUntilNewAnimation * _numPixel)) {
        _currentAnimationCycle = 0;
        //LOGGER.println("Reset _currentAnimationCycle");
        // evtl hier auch noch Animations-Schritte dazwischen berechnen
    }

}



void KSWS2812B::setBrightness(uint8_t brightness) {
    csPixelsAccess.EnterCriticalSection();
    _currentBrightness = brightness;
    _neoPixel.setBrightness(_currentBrightness);          // mit critical Section absichern
    csPixelsAccess.LeaveCriticalSection();
}



void KSWS2812B::setLightEffect(LightEffect effect) {

    if (effect < LightEffect::LENumOfLightEffects) {
        _currentLightEffect = effect;
        setAnimationTimeMS(lightEffects[(int)effect].defaultAnimationTime);
        updateLights();
        //LOGGER.printf("setLightEffect(%s): %s\n", lightEffects[(int)effect].pEffectName, lightEffects[(int)effect].pEffectDescription);
    }
}


void KSWS2812B::setLightsOff() {
    csPixelsAccess.EnterCriticalSection();
    _neoPixel.clear();
    _neoPixel.show();   // Send the updated pixel colors to the hardware.   
    csPixelsAccess.LeaveCriticalSection();
    _lightsOff = true;
}


void KSWS2812B::setLightsOn() {
    _lightsOff = false;
    _lightEffectChanged = true;
}


void KSWS2812B::setAllPixelToColor(uint8_t r, uint8_t g, uint8_t b) {
    uint32_t color = _neoPixel.Color(r, g, b);
    setAllPixelToColor(color);
}



void KSWS2812B::setAllPixelToColor(uint32_t color) {
    csPixelsAccess.EnterCriticalSection();
    for (int i = 0; i < _numPixel; i++) { // For each pixel...
        _neoPixel.setPixelColor(i, color);
    }
    _neoPixel.show();   // Send the updated pixel colors to the hardware.   
    setColor(color);
    csPixelsAccess.LeaveCriticalSection();
}




void KSWS2812B::setRainbow(uint16_t firstPixel, uint16_t first_hue, int8_t reps, uint8_t saturation, uint8_t brightness, bool gammify) {
    csPixelsAccess.EnterCriticalSection();
    for (uint16_t i = 0; i < _numPixel; i++) {
        uint16_t hue = first_hue + (i * reps * 65536) / _numPixel;

        uint32_t color = _neoPixel.ColorHSV(hue, saturation, brightness);
        if (gammify) color = _neoPixel.gamma32(color);
        
        uint16_t realPixel = (i + firstPixel) % _numPixel;      // use first Pixel as Offset and start after _numPixel at 0
        _neoPixel.setPixelColor(realPixel, color);
    }
    _neoPixel.show();   // Send the updated pixel colors to the hardware.   
    csPixelsAccess.LeaveCriticalSection();
}




void KSWS2812B::setColor(uint8_t r, uint8_t g, uint8_t b) {
    setColor(_neoPixel.Color(r, g, b));
}



void KSWS2812B::setColor(uint32_t color) {
    _lastColor = color;
}



#endif      // #if defined USE_KSWS2812B || defined KSLIBRARIES_USEALL