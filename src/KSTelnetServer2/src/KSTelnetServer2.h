/**
 * @file KSTelnetServer2.h
 * 
 * @brief Header file for KSTelnetServer2 class
 * 
 * @details 
 * 
 * @see
 *   - lennarthennigs/ESP Telnet @ ^2.1.2
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


#ifndef _KSTELNETSERVER2_H
#define _KSTELNETSERVER2_H


#include "KSESPTelnet.h"


class KSTelnetServer2 : public Print {
    public:
        KSTelnetServer2(uint16_t port = 23);
        ~KSTelnetServer2();

        TaskHandle_t createConnection(EventGroupHandle_t *phEventGroupNetwork = NULL);
 		
        void setDefaultCommandHandler(bool handleDefault) { _handleDefaultCommands = handleDefault; }
        void setOnReadCommandListener(void (*callback)(char* szCommand)) { onReadCommandListener = callback; }
        void setOnReadSetCommandListener(void (*callback)(char* szParameter, char* szValue)) { onReadSetCommandListener = callback; }
    
        bool shouldSendCyclicData() { return _sendCyclicData; }
        void setProjectAndVersion(char* pProject, char* pVersion) { _pPROJECT_NAME = pProject; _pSW_VERSION = pVersion; }
        
        static void onInputReceived(String str);
        static void onConnect(String ip);
        static void onConnectionAttempt(String ip);
        static void onReconnect(String ip);
        static void onDisconnect(String ip);

        static KSTelnetServer2* obj;     // for singleton. Used for callbacks

     	virtual size_t write(uint8_t b) override {
            return write(&b, 1);
        }
        virtual size_t write(const uint8_t *buf, size_t size) override {
            size_t ret = 0;
            ret = _telnet.write(buf, size);
            return ret;
        }

        using Print::write;

        void flush() {
            _telnet.flush();
        }


    protected:
       void (*onReadCommandListener)(char* szCommand);
       void (*onReadSetCommandListener)(char* szParameter, char* szValue);

       void setupCallbacks();

    private:
        void tKSTelnetServer();

        TaskHandle_t _htKSTelnetServer;
        EventGroupHandle_t *_phEventGroupNetwork = NULL;

        bool _handleDefaultCommands = true;
        bool _sendCyclicData = false;

        uint16_t _port = 23;
        KSESPTelnet _telnet;

        static char* _pSW_VERSION;
        static char* _pPROJECT_NAME;
};


extern KSTelnetServer2 Telnet2;



#endif  // #define _KSTELNETSERVER2_H