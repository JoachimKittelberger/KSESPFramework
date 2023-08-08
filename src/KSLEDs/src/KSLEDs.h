/**
 * @file KSLEDs.h
 *
 * @brief Header file for KSLEDs class
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

#ifndef _KSLEDS_H
#define _KSLEDS_H

#include <Arduino.h>


#define KSLED_OFF false
#define KSLED_ON true


const int PWM_CHANNEL_RED = 0;
const int PWM_CHANNEL_GREEN = 1;
const int PWM_CHANNEL_BLUE = 2;
const int PWM_FREQ  = 5000;
const int PWM_RESOLUTION = 8;

// TODO: Channel auf 255 setzen und nicht mit 100% arbeiten

class KSLEDs
{
    public:
        KSLEDs(uint8_t pinLEDRed, uint8_t pinLEDGreen, uint8_t pinLEDBlue);
        ~KSLEDs() {};
  
        void init();

        void setRedLED(bool newState = false, int brightness = 100);
        void setGreenLED(bool newState = false, int brightness = 100);
        void setBlueLED(bool newState = false, int brightness = 100);

        void setRGB(bool newState, int brightnessRed = 100, int brightnessGreen = 100, int brightnessBlue = 100);

    private:
        int getDutyCycleFromPercent(int percent);    

        uint8_t _pinLEDRed;
        uint8_t _pinLEDGreen;
        uint8_t _pinLEDBlue;

        int _brightnessLEDRed = 0;
        int _brightnessLEDGreen = 0;
        int _brightnessLEDBlue = 0;
        const int MAX_DUTY_CYCLE = (int)(pow(2, PWM_RESOLUTION) -1);

};


#endif  // #define _KSLEDS_H