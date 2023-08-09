/**
 * @file KSLogger.h
 * 
 * @brief Header file for using KSLogger
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

#ifndef _KSLOGGER_H
#define _KSLOGGER_H

#include <Arduino.h>

#if !defined(USE_TELNET1)
    #define USE_TELNET2
#endif

#if defined(USE_TELNET2)
    #include "KSTelnetServer2/src/KSTelnetServer2.h"
#else
    #include "KSTelnetServer/src/KSTelnetServer.h"
#endif


// use Serial as Standard for Logging
#if !defined(LOG_SERIAL) && !defined(LOG_TELNET) && !defined(LOG_SERIAL_AND_TELNET)
    #define LOG_SERIAL
#endif

#if defined(LOG_SERIAL)
    #define LOGGER  Serial
#elif defined(LOG_TELNET)
    #if defined(USE_TELNET2)
        #define LOGGER  Telnet2
    #else
        #define LOGGER  Telnet
    #endif
#elif defined(LOG_SERIAL_AND_TELNET)
    #define LOGGER  SerialAndTelnet
#endif


class KSSerialAndTelnet : public Stream {

    public:
#if defined(USE_TELNET2)
        KSSerialAndTelnet(KSTelnetServer2* telnet = NULL) {
            _telnet = telnet;
        };
#else
        KSSerialAndTelnet(KSTelnetServer* telnet = NULL) {
            _telnet = telnet;
        };
#endif
        bool init();
        // Stream implementation
        int read();
        int available();
        int peek();


        // Print implementation
        virtual size_t write(uint8_t val);
        virtual size_t write(const uint8_t *buf, size_t size);
        using Print::write; // pull in write(str) and write(buf, size) from Print
        virtual void flush();

    private:
#if defined(USE_TELNET2)
       KSTelnetServer2* _telnet = NULL;
#else
        KSTelnetServer* _telnet = NULL;
#endif

};


extern KSSerialAndTelnet SerialAndTelnet;




#endif  // #define _KSLOGGER_H