/**
 * @file KSAppInfo.h
 * 
 * @brief Header for KSAppInfo class
 * 
 * @details handles application information
 * 
 * @see
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

#ifndef _KSAPPINFO_H
#define _KSAPPINFO_H

#include <Arduino.h>


#include <WiFi.h>           // for getDeviceName
#include <ArduinoJson.h>


extern const char SW_VERSION[];
extern const char PROJECT_NAME[];


class KSAppInfo
{
    public:
        KSAppInfo() {
            // get mac address and extend the deviceName with it
            byte mac[6];
            WiFi.macAddress(mac);
            char macStr[18];
            snprintf(&macStr[0], sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
            strncpy(&_deviceName[6], macStr, 18);
        };
        ~KSAppInfo() {};
  
        void init() {};

        const char* getDeviceName() { return &_deviceName[0]; }
        const char* getProjectName() { return PROJECT_NAME; }
        const char* getCurrentVersion() { return SW_VERSION; }
        String getIPAddress() { return WiFi.localIP().toString(); }
        String getDeviceType() { return ESP.getChipModel(); }
        
        String listESPStateJSON();


    private:
        char _deviceName[24] = { 'J', 'K', 'E', 'S', 'P', '-',          // TODO: Hier ein min 8-Byte langer NAme erlauben oder Speicher dynamisch anfordern
                                0 ,0, 0, 0, 0, 0, 0, 0, 0,              // space for mac-Address
                                0, 0, 0, 0, 0, 0, 0, 0, 0 };
 
};


#endif  // #define _KSAPPINFO_H