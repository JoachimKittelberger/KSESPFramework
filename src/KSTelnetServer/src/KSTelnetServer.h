/**
 * @file KSTelnetServer.h
 * 
 * @brief Definition of KSTelnetServer class
 * 
 * @details
 * 
 * @see
 * 
 * @author Joachim Kittelberger <jkittelberger@kibesoft.de>
 * @date 24.04.2022
 * @version 1.01
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

#ifndef _KSTELNETSERVER_H
#define _KSTELNETSERVER_H


#include <WiFi.h>
#include "TelnetStream.h"


/*
Bedeutung der Kommandos:
255 - IAC - Interpret as Command (2-3 Bytes)
Subcommands 251 - 254 3 Bytes
Subcommands 240 - 250 2 Bytes

Nach Text ENTER -> 13, 10 wird gesendet

*/

static const char TELNET_IAC = 255;

static const char TELNET_WILL = 251;
static const char TELNET_WONT = 252;
static const char TELNET_DO = 253;
static const char TELNET_DONT = 254;

static const char TELNET_CR = 13;
static const char TELNET_LF = 10;

#define TELNET_CMD_LENGTH 32


class KSTelnetServer : public TelnetStreamClass {
    public:
        KSTelnetServer(uint16_t port);
        ~KSTelnetServer();

        TaskHandle_t createConnection(EventGroupHandle_t *phEventGroupNetwork = NULL);
 		
        void setDefaultCommandHandler(bool handleDefault) { _handleDefaultCommands = handleDefault; }
        void setOnReadCommandListener(void (*callback)(char* szCommand)) { onReadCommandListener = callback; }
        void setOnReadSetCommandListener(void (*callback)(char* szParameter, char* szValue)) { onReadSetCommandListener = callback; }
    
        bool shouldSendCyclicData() { return _sendCyclicData; }

    protected:
       void (*onReadCommandListener)(char* szCommand);
       void (*onReadSetCommandListener)(char* szParameter, char* szValue);

       bool readNextCharWithTimeout(char& ch, int timeout);


    private:
        void tKSTelnetServer();

        TaskHandle_t _htKSTelnetServer;
        EventGroupHandle_t *_phEventGroupNetwork = NULL;

        bool _handleDefaultCommands = true;
        bool _sendCyclicData = false;
        char _szTelnetCmd[TELNET_CMD_LENGTH + 1] = { 0 };      // maximum TELNET_CMD_LENGTH
};


extern KSTelnetServer Telnet;



#endif  // #define _KSTELNETSERVER_H