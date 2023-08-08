/**
 * @file KSWS2812B.h
 * 
 * @brief Header file for using WS2812B
 * 
 * @details 
 * 
 * @see
 *   - 	adafruit/Adafruit NeoPixel @ ^1.11.0            https://github.com/adafruit/Adafruit_NeoPixel.git
 * 
 * @author Joachim Kittelberger <jkittelberger@kibesoft.de>
 * @date 20.06.2022
 * @version 1.00
 *
 * @todo
 * - Weitere effecte einbauen unter:
 *   - https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/
 *   - https://github.com/kitesurfer1404/WS2812FX
 *   - https://adrianotiger.github.io/Neopixel-Effect-Generator/
 *   - https://kno.wled.ge/advanced/custom-features/
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

#ifndef _KSWS2812B_H
#define _KSWS2812B_H



#include <Adafruit_NeoPixel.h>
#include "KSCriticalSection/src/KSCriticalSection.h"

// TODO: Evtl. die einzelnen Effekte in Klassen packen, die ihr Verhalten dann selber steuern können
// TODO: Enum und Struct umbenennen und bei get die Struktur zurückgeben


// if we add new effect we have also add them in
// const LEStruct lightEffects[LE_NUMBER_OF_EFFECTS]
// void KSWS2812B::handle()
enum LightEffect {
    LEBlack = 0,
    LEWhite,
    LERed,
    LEGreen,
    LEBlue,
    LELastColor,
    LERainbow,
    LERainbowCycle,


    LENumOfLightEffects
};




typedef struct {
    LightEffect leEffect;
    const char* pEffectName;
    const char* pEffectDescription;
    uint32_t defaultAnimationTime;
    // hier noch defaultspeed ... aufnehmen
} LEStruct;



extern const LEStruct lightEffects[LightEffect::LENumOfLightEffects];




class KSWS2812B
{
    public:
        KSWS2812B(int numPixel, int pin);
        ~KSWS2812B() {};

        TaskHandle_t create();

        void setBrightness(uint8_t brightness);
        uint8_t getBrigthness() { return _currentBrightness; }

        void setLightEffect(LightEffect effect);
        LightEffect getLightEffect() { return _currentLightEffect; }
        void updateLights() { _lightEffectChanged = true; }

        void setLightsOff();
        void setLightsOn();
        bool areLightsOn() { return !_lightsOff; }

        void setAnimationTimeMS(int timeMS) {
            if (timeMS >= _currentScheduleTimer) _currentAnimationTimeMS = timeMS;
            else _currentAnimationTimeMS = _currentScheduleTimer;
        }
        int getAnimationTimeMS() { return _currentAnimationTimeMS; }


        void setColor(uint8_t r, uint8_t g, uint8_t b);
        void setColor(uint32_t color);
        uint32_t getColor() { return _lastColor; }

    protected:
        bool init();            // could be used without create() (without separat thread). But must be make public in this case
        void handle();          // could be used without create() (without separat thread). But must be make public in this case

        void setAllPixelToColor(uint8_t r, uint8_t g, uint8_t b);
        void setAllPixelToColor(uint32_t color);
        void setRainbow(uint16_t firstPixel, uint16_t first_hue, int8_t reps, uint8_t saturation, uint8_t brightness, bool gammify); 


    private:
        volatile LightEffect _currentLightEffect = LightEffect::LEBlack;
        volatile uint32_t _lastColor = 0xFF0000;         // default is red

        volatile bool _lightEffectChanged = true;
        volatile uint8_t _currentBrightness = 128;
        int _currentAnimationCycle = 0;
        int _currentAnimationTimeMS = 200;      // Time for Animation-Change
        volatile bool _lightsOff = false;

        Adafruit_NeoPixel _neoPixel;

        void tKSWS2812B();
        TaskHandle_t _htKSWS2812B;
        volatile int _currentScheduleTimer = 20;         // default is 20ms for scheduling Animation-Task

        int _numPixel = 0;                          // Number of LEDs in stripe
        int _pin = 0;                               // Pin for WS2812B-Stripe Data

        KSCriticalSection csPixelsAccess;			// Critical Section for access to Jet32-API and synchronize calls
};






#endif  // #define _KSWS2812B_H