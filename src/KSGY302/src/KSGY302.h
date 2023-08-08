/**
 * @file KSGY302.h
 *
 * @brief Header file for KSGY302 class
 * 
 * @details 
 * 
 * @see
 *   - https://github.com/Erriez/ErriezBH1750.git
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

#ifndef _KSGY302_H
#define _KSGY302_H

#include <Arduino.h>
#include <ErriezBH1750.h>

#include "KSCriticalSection/src/KSCriticalSection.h"





class KSGY302 {
    public:
        // ADDR line LOW/open:  I2C address 0x23 (0x46 including R/W bit) [default]
        // ADDR line HIGH:      I2C address 0x5C (0xB8 including R/W bit)
        KSGY302(uint8_t addrPinLevel = LOW, KSCriticalSection* pcsI2C = NULL);
        ~KSGY302() {};

        float readIlluminance();
        int readRawData();
        void sleep();
    
    protected:
        void init(BH1750_Mode_e mode, BH1750_Resolution_e resolution);
 
    private:
        BH1750 _sensor;
        KSCriticalSection* _pcsI2C = NULL;

};


#endif  // #define _KSGY302_H