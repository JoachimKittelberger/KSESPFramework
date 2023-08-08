/**
 * @file KSLiquidCrystal_I2C.h
 * 
 * @brief Header file for using KSLiquidCrystal_I2C
 * 
 * @details 
 * 
 * @see
 *   - https://github.com/iakop/LiquidCrystal_I2C_ESP32.git
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
#ifndef _KSLIQUIDCRYSTAL_I2C_H
#define _KSLIQUIDCRYSTAL_I2C_H

#include <LiquidCrystal_I2C.h>

#include "KSCriticalSection/src/KSCriticalSection.h"


class KSLiquidCrystal_I2C : public LiquidCrystal_I2C {
    public:
        KSLiquidCrystal_I2C(uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows, KSCriticalSection* pcsI2C = NULL);

        KSCriticalSection* _pcsI2C = NULL;
};



#endif  // #define _KSLIQUIDCRYSTAL_I2C_H