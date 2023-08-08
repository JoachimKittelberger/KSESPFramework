/**
 * @file KSPreferences.cpp
 * 
 * @brief Implementation file for using KSPreferences
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
 *   - Wenn Passwort zur Laufzeit geändert wird, dies dann auch anpassen, damit es gleich verwendet werden kann
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


#if defined USE_KSPreferences || defined KSLIBRARIES_USEALL     // include File in Build only if it ist defined to use it


#include "KSPreferences.h"

#include <nvs_flash.h>


/*
extern const char wifi_ssid[];
extern const char wifi_password[];

extern const char fbUsername[];
extern const char fbPassword[];
extern const char fbIp[];

extern const char mqttServerIP[];
extern const char mqttUserName[];
extern const char mqttUserPassword[];

extern const char ftpUserName[];
extern const char ftpUserPassword[];

*/



void KSPreferences::putKSValues() {
    eraseNVSMemory();               // erase complete NVS memory

/*
    // credenntials for wifi
    begin("wifi", false);
	putString("ssid", wifi_ssid); 
	putString("password", wifi_password);
    end();

    // credentials for fritzbox access
    begin("fritzbox", false);
	putString("username", fbUsername); 
	putString("password", fbPassword);
	putString("ip", fbIp);
    end();

    // credentials for mqtt server
    begin("mqtt", false);
	putString("ip", mqttServerIP); 
	putString("username", mqttUserName);
	putString("password", mqttUserPassword);
    end();

    // credentials for ftp server
    begin("ftp", false);
	putString("username", ftpUserName); 
	putString("password", ftpUserPassword);
    end();
    */
}



// erase the complete NVS-Memory
void KSPreferences::eraseNVSMemory() {
    nvs_flash_erase(); // erase the NVS partition and...
    nvs_flash_init(); // initialize the NVS partition.
}


#endif      // #if defined USE_KSPreferences || defined KSLIBRARIES_USEALL