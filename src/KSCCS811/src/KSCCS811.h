/**
 * @file KSCCS811.h
 * 
 * @brief Header for KSCCS811 class
 * 
 * @details
 * 
 * @see
 *   - Used Library: https://github.com/adafruit/Adafruit_CCS811.git
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

#ifndef _KSCCS811_H
#define _KSCCS811_H

#include <Adafruit_CCS811.h>
#include "KSCriticalSection/src/KSCriticalSection.h"


class KSCCS811
{
    public:
        KSCCS811(KSCriticalSection* pcsI2C = NULL);
        ~KSCCS811() {};

        void init();

        int readeCO2();
        int readTVOC();
        bool read(int* peCO2 = NULL, int* pTVOC = NULL);

    private:
        Adafruit_CCS811 _ccs811;

        KSCriticalSection* _pcsI2C = NULL;
};


#endif  // #define _KSCCS811_H