/**
 * @file KSPreferences.h
 * 
 * @brief Header file for using KSPreferences
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


#pragma once

#include <Preferences.h>


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



class KSPreferences : public Preferences {
    public:
        KSPreferences() : Preferences() {}
        virtual ~KSPreferences() { }

        void eraseNVSMemory();           // erase the complete NVS-Memory
        void putKSValues();              // initialize all KSValues


    protected:
 
    private:
};




/*
	preferences für credentials wifi, mqtt, ....... generell in einem Setup-Programm anlegen. Evtl. eine KSPreferences-Lib mit Anlegen, löschen, ...


	preferences.begin("weatherstation", false);
	//preferences.clear();	// clear all data under namespace
	//preferences.remove(key);	// remove key from an open namespace

	unsigned int counter = preferences.getUInt("counter", 0);
	counter++;
	preferences.putUInt("counter", counter);
	preferences.end();				// close the opened namespace
*/

