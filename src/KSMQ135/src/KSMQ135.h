/**
 * @file KSMQ135.h
 * 
 * @brief Header file for using KSMQ135 class
 * 
 * @details 
 * 
 * @see
 *   - used library: https://github.com/ViliusKraujutis/MQ135.git
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

#ifndef _KSMQ135_H
#define _KSMQ135_H


#include <MQ135.h>

#define RLOAD 15000  // sollte vermutlich eher 15k sein
#define RZERO 6500
//#define RZERO 1     // Für Calibrierung??

// #define Voltage_Resolution 3.3
// #define ADC_Bit_Resolution 12           // For ESP32




class KSMQ135
{
    public:
        KSMQ135(uint8_t anaPin);
        ~KSMQ135() {};

        void init();

        float readCO2();
        bool read(float* pCO2 = NULL);

    private:
        MQ135 _mq135;
        uint8_t _anaPin = -1;
};


#endif  // #define _KSMQ135_H