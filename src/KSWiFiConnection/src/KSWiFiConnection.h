/**
 * @file KSWiFiConnection.h
 * 
 * @brief Definition of KSWiFiConnection class
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


#ifndef _KSWIFICONNECTION_H
#define _KSWIFICONNECTION_H

#if defined(ESP8266)
    #include <ESP8266WiFi.h>
#elif defined(ESP32)
    #include <WiFi.h>
#else
#error "This isn't a ESP8266 or ESP32!"
#endif


extern const char wifi_ssid[];
extern const char wifi_password[];


class KSWiFiConnection {
    public:
        KSWiFiConnection(const char* pSSID = wifi_ssid, const char* pPassword = wifi_password, uint8_t pinLEDWifiStatus = 0);
        ~KSWiFiConnection();

        TaskHandle_t createConnection(EventGroupHandle_t *phEventGroupNetwork = NULL);
        bool waitForInit(TickType_t xTicksToWait = portMAX_DELAY);
 
        void setStaticConfig(const char* pIP, const char* pGateway, const char* pSubnet, const char* pDns);
        void enableStatusLEDs(bool enable = true) { _enableLEDs = enable; }
        void setCredentials(const char* pSSID, const char* pPassword) { _pSSID = const_cast<char*>(pSSID); _pPassword = const_cast<char*>(pPassword); }

    private:
        void tKSWiFiConnection();
        
        static void WiFiEvent(WiFiEvent_t event);

        TaskHandle_t _htKSWiFiConnection;
        EventGroupHandle_t *_phEventGroupNetwork = NULL;


        bool _useDHCP = true;

        IPAddress _staticIP;      // statische IP des NodeMCU ESP8266
        IPAddress _staticGateway;        // IP-Adresse des Router
        IPAddress _staticSubnet;         // Subnetzmaske des Netzwerkes
        IPAddress _staticDns;              // DNS Server

        // Status LED for WiFi-Status. Blinking = connecting, on = connected, off = disconnected
        bool _enableLEDs = false;
        uint8_t _pinLEDWifiStatus = 0;
        
        const char* _pSSID = wifi_ssid;
        const char* _pPassword = wifi_password;
};




#endif // #define _KSWIFICONNECTION_H