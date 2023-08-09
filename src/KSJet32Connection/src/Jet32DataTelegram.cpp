/**
 * @file Jet32DataTelegram.cpp
 *
 * @brief Implementation file for Jet32DataTelegram class
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

#if defined USE_KSJet32Connection || defined KSLIBRARIES_USEALL     // include File in Build only if it ist defined to use it

#include "Jet32DataTelegram.h"

#include "KSLogger/src/KSLogger.h"



Jet32DataTelegram::Jet32DataTelegram(int receivePort) : 
   _jet32CommandData {
        0x4A, 0x57, 0x49, 0x50,
        0x00, 0x01, 0x00, 0x01,         // protocol version 1.1
        0x00, 0x00, 0x00, 0x00,         // communication reference
        0x00, 0x00, 0x00, 0x00,         // receive Port
        0x00, 0x00, 0x00, 0x00,         // reserved
        
        0x00,                           // set Output
        0x00,                           // modifier byte
        0x00, 0x00, 0x00, 0x00,         // number of Output
        0x00, 0x00, 0x00, 0x00          // index
    }
{ 
    _receivePort = receivePort;
};
 


void Jet32DataTelegram::printArray(uint8_t* rawarray, uint16_t length) {
//    LOGGER.println("Raw-Values:");
    for (uint16_t i = 0; i < length; i++) {
        uint8_t value = rawarray[i];
//        LOGGER.printf("%u " ,value);
        LOGGER.printf("0x%02X " ,value);
    }
}



void Jet32DataTelegram::init(Jet32Command cmd, uint32_t number, int32_t value, uint32_t comRef) {
    // general Data
    _jet32CommandData[8] = uint8_t(comRef >> 24);
    _jet32CommandData[9] = uint8_t(comRef >> 16);
    _jet32CommandData[10] = uint8_t(comRef >> 8);
    _jet32CommandData[11] = uint8_t(comRef);

    _jet32CommandData[12] = uint8_t(_receivePort >> 24);
    _jet32CommandData[13] = uint8_t(_receivePort >> 16);
    _jet32CommandData[14] = uint8_t(_receivePort >> 8);
    _jet32CommandData[15] = uint8_t(_receivePort);

    _jet32CommandData[20] = (uint8_t)cmd;

    switch (cmd) {
        case Jet32Command::connect: 
            _jet32CommandData[21] = 0x11;       // testbyte. this will return as XOR FFH -> EEH
            _length = 22;
            break;

        case Jet32Command::setOutput: 
        case Jet32Command::clearOutput: 
            _jet32CommandData[21] = 0;

            _jet32CommandData[22] = uint8_t(number >> 24);
            _jet32CommandData[23] = uint8_t(number >> 16);
            _jet32CommandData[24] = uint8_t(number >> 8);
            _jet32CommandData[25] = uint8_t(number);

            _jet32CommandData[26] = 0;
            _jet32CommandData[27] = 0;
            _jet32CommandData[28] = 0;
            _jet32CommandData[29] = 0;
            _length = 30;
            break;
            
   
       case Jet32Command::writeIntRegister: 
            _jet32CommandData[21] = uint8_t(number >> 16);
            _jet32CommandData[22] = uint8_t(number >> 8);
            _jet32CommandData[23] = uint8_t(number);

            _jet32CommandData[24] = uint8_t(value >> 24);
            _jet32CommandData[25] = uint8_t(value >> 16);
            _jet32CommandData[26] = uint8_t(value >> 8);
            _jet32CommandData[27] = uint8_t(value);
            _length = 28;
            break;
    
       case Jet32Command::readIntRegister: 
            _jet32CommandData[21] = uint8_t(number >> 16);
            _jet32CommandData[22] = uint8_t(number >> 8);
            _jet32CommandData[23] = uint8_t(number);
            _length = 24;
            break;

    
       case Jet32Command::readFlag: 
       case Jet32Command::setFlag: 
       case Jet32Command::resetFlag: 
            _jet32CommandData[21] = uint8_t(number >> 8);
            _jet32CommandData[22] = uint8_t(number);
            _length = 23;
            break;

       case Jet32Command::readVariable: 
            _jet32CommandData[21] = uint8_t(number >> 24);
            _jet32CommandData[22] = uint8_t(number >> 16);
            _jet32CommandData[23] = uint8_t(number >> 8);
            _jet32CommandData[24] = uint8_t(number);
            _length = 25;
            break;

       case Jet32Command::writeVariable: 
            _jet32CommandData[21] = uint8_t(number >> 24);
            _jet32CommandData[22] = uint8_t(number >> 16);
            _jet32CommandData[23] = uint8_t(number >> 8);
            _jet32CommandData[24] = uint8_t(number);

            _jet32CommandData[25] = 0x11;           // DataType long
            
            _jet32CommandData[26] = uint8_t(value >> 24);
            _jet32CommandData[27] = uint8_t(value >> 16);
            _jet32CommandData[28] = uint8_t(value >> 8);
            _jet32CommandData[29] = uint8_t(value);
            _length = 30;
            break;

        default:
            // not supported Command
            LOGGER.printf("CMD %u not supported", cmd);
            break;
    }
}


#endif      // #if defined USE_KSJet32Connection || defined KSLIBRARIES_USEALL