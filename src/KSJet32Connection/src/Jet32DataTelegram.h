/**
 * @file Jet32DataTelegram.h
 *
 * @brief Header file for Jet32DataTelegram class
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

#ifndef _JET32DATATELEGRAM_H
#define _JET32DATATELEGRAM_H


#include <Arduino.h>



enum class Jet32Command : uint8_t {
    connect = 0x55,
    setOutput = 0x87,
    clearOutput = 0x88,
    writeIntRegister = 0x52,
    readIntRegister = 0x51,
    readFlag = 0x4B,
    setFlag = 0x4C,
    resetFlag = 0x4D,
    readVariable = 0x7C,
    writeVariable = 0x7D
};



#define JET32_MAXTELEGRAMLENGTH 30

class Jet32DataTelegram {
    public:
        Jet32DataTelegram(int receivePort = 0);
        ~Jet32DataTelegram() {};

        void init(Jet32Command cmd, uint32_t number, int32_t value, uint32_t comRef);


        void printData() { printArray(_jet32CommandData, _length); };
        
        int Length() { return _length; };
        uint8_t* RawDataPtr() { return _jet32CommandData; };

        static void printArray(uint8_t* rawarray, uint16_t length);

    private:

        // maximum lenght is header[20] + data[10]+/-  // maximum lenght is 30
        uint8_t _jet32CommandData[JET32_MAXTELEGRAMLENGTH];
 
        int _length = JET32_MAXTELEGRAMLENGTH;                   // maximum lenght is 30
        int _receivePort = 0;

};




#endif      // #ifndef _JET32DATATELEGRAM_H