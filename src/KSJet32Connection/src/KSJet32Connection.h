/**
 * @file KSJet32Connection.h
 *
 * @brief Header file for KSJet32Connection class
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

#ifndef _KSJET32CONNECTION_H
#define _KSJET32CONNECTION_H


#include <WiFi.h>
#include <WiFiUdp.h>

#include "Jet32DataTelegram.h"
#include "KSCriticalSection/src/KSCriticalSection.h"




class KSJet32Connection {
    public:
        KSJet32Connection(IPAddress remoteIP, int sendPort, int recPort) {
            _remoteIP = remoteIP;
            _sendPort = sendPort;
            _recPort = recPort;
        };
        ~KSJet32Connection() {
            disconnect();
        };

        TaskHandle_t createConnection(EventGroupHandle_t *phEventGroupNetwork = NULL);

        bool getConnectionState(int8_t value);
        int32_t readIntRegister(uint32_t number);
        bool writeIntRegister(uint32_t number, int32_t value);
        void setTimeOut(int timeoutInMS) { _timeoutInMS = timeoutInMS; };

    protected:
        bool sendTelegramSynchron(Jet32DataTelegram* pMsg, uint32_t comRef, bool bCheckConnected = true);
        bool sendTelegramAsynchron(Jet32DataTelegram* pMsg, uint32_t comRef, bool bCheckConnected = true);
        void processPacketHandler(uint8_t* packet, int length);
        bool waitForResponse(int timeoutInMS);

        bool connect();
        void disconnect();
        bool isConnected();
       
    private:
        void tKSJet32Connection();

        void resetConnectedEvent();
        void resetConnectingEvent();
        void setConnectedEvent();
        void setConnectingEvent();

        TaskHandle_t _htKSJet32Connection;
        EventGroupHandle_t *_phEventGroupNetwork = NULL;

        WiFiUDP _udpReceive;
        WiFiUDP _udpSend;

        IPAddress _remoteIP;
        int _sendPort = 0;
        int _recPort = 0;
    
        KSCriticalSection csJet32Access;			// Critical Section for access to Jet32-API and synchronize calls
        int _timeoutInMS = 2000;                    // Default TimeOut

        volatile bool _bNewValueArrived = false;
        volatile int32_t _lastValue = 0;
        volatile bool _bConnected = false;
};







#endif  // #ifndef _KSJET32CONNECTION_H