/**
 * @file TR064Helper.h
 * 
 * @brief Header for TR064 Helper Functions
 * 
 * @details handles communication to fritz.box homeautomation-switches
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


#pragma once
#include <Arduino.h>

#if defined(ESP8266)
	//Imports for ESP8266
	#include <ESP8266WiFi.h>
	#include <ESP8266HTTPClient.h>
#elif defined(ESP32)
	//Imports for ESP32
	#include <WiFi.h>
	#include <HTTPClient.h>
#endif

#include <tr064.h>





/*
 * The array of macs. Structure:
 * = {{"mac1:user1", "mac2:user1", ..., "macX:user1"}, {"mac1:user2", "mac2:user2", ..., "macX:user2"}, ..., {"mac1:userY", "mac2:userY", ..., "macX:userY"}};
 * Doesn't matter if upper or lowercase :)
 */
/*
const char macsPerUser[numUser][maxDevices][18] =
    { {"01:23:45:67:89:AB","12:34:56:78:9A:BC"}, //User1, two devices
      {"23:45:67:89:AB:CD"}, //User2, one device
      {"34:56:78:9A:BC:DE", "45:67:89:AB:CD:EF", "56:78:9A:BC:DE:F0"}}; //User3, three devices
*/

// Array-settings. No need to change these!
const String STATUS_MAC = "MAC";
const String STATUS_IP = "IP";
const String STATUS_ACTIVE = "ACTIVE";
const String STATUS_HOSTNAME = "HOSTNAME";
const int STATUS_MAC_INDEX = 0;
const int STATUS_IP_INDEX = 1;
const int STATUS_ACTIVE_INDEX = 3;
const int STATUS_HOSTNAME_INDEX = 2;







int getWifiNumber();

void getStatusOfAllWifi();

void getStatusOfAllWifi(int numDev);

void getStatusOfMACwifi(String mac, String (&r)[4][2]);

int getDeviceNumber();

void getStatusOfMAC(String mac, String (&r)[4][2]);

void verboseStatus(String r[4][2]);

