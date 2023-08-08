/**
 * @file KSHomeKit.h
 *
 * @brief Header file for KSHomeKit class
 * 
 * @details 
 * 
 * @see
 *   - https://github.com/HomeSpan/HomeSpan.git
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
#pragma once

#include "HomeSpan.h" 



extern const char SW_VERSION[];
extern const char wifi_ssid[];
extern const char wifi_password[];
extern const char hkDefaultSetupCode[];




class KSHomeKit {
    public:
        KSHomeKit(const char* hostname = NULL, const char* sketchVersion = SW_VERSION);
        ~KSHomeKit();

        TaskHandle_t createHomeKit();

        void setStaticConfig(const char* pIP, const char* pGateway, const char* pSubnet, const char* pDns);

        void setOnWifiEstablishedListener(void (*callback)()) {
            homeSpan.setWifiCallback(callback);
        }
        void setOnSetupHomeSpanListener(void (*callback)()) {
            onSetupHomeSpanListener = callback;
        }

        void EraseFlashData() { homeSpan.processSerialCommand("E"); }

    protected:
        void wifiEstablished();
       
        void (*onSetupHomeSpanListener)();

    private:
        void tKSHomeKit();

        bool _useDHCP = true;
        IPAddress _staticIP;                // statische IP des NodeMCU ESP8266
        IPAddress _staticGateway;           // IP-Adresse des Router
        IPAddress _staticSubnet;            // Subnetzmaske des Netzwerkes
        IPAddress _staticDns;               // DNS Server

        const char* _hostname;
        const char* _sketchVersion;

        TaskHandle_t _htKSHomeKit;
};


