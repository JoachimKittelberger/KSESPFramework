/**
 * @file myCredentials.h
 * 
 * @brief Definitions of credentials for KSESPLibraries
 * 
 * @details
 *   This file must be copied in the include-Dir of your project an renamend to myCredentials.h and included in the main programm
 *   Adapt the settings to your local needs
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
#ifndef _KSCREDENTIALS_H
#define _KSCREDENTIALS_H


// This is the SSID and password for your WIFI access point, change as desired
#ifdef USE_LOCAL_AP
const char wifi_ssid[] = "SSID_AP";
const char wifi_password[] = "password";
#else
const char wifi_ssid[] = "SSID";
const char wifi_password[] = "password";
#endif


// fritzbox settings
const char fbUsername[] = "user";
const char fbPassword[] = "password";
const char fbIp[] = "192.168.1.1";


// This is the mqtt server and password
const char mqttServerIP[] = "192.168.1.2";
const char mqttUserName[] = "user";
const char mqttUserPassword[] = "password";


// This is the ftp server username and password
const char ftpUserName[] = "user";
const char ftpUserPassword[] = "password";


// this ist not stored in Preferences
// These are the homekit credentials and infos
const char hkManufacturer[] = "MyCompany";
const char hkDefaultSetupCode[] = "12345678";
const char HAPProtoInfoVersion[] = "1.0.0";



// This is the mqtt server and password
const char updateServerIP[] = "192.168.1.3";         // MAC-Book Joachim
const char updateUserName[] = "admin";
const char updateUserPassword[] = "password";


// NTP-Settings
const char ntpServer[] = "de.pool.ntp.org";
//const char ntpServer[] = "pool.ntp.org";
const long gmtOffset_sec = 1*60*60;		// UTC + 1.00
const int daylightOffset_sec = 3600;	// Sommerzeit 3600 ansonsten 0
// für KSNTPClietnLight
const char timezoneGermany[] = "CET-1CEST,M3.5.0,M10.5.0/3";           // Timezone for Berlin
//#define TZ_INFO "WEST-1DWEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00" // Western European Time


// sunrise and sunset calculation with latitude and longitude
/// Latitude value for calculating the sunset and sunrise in Kirchbberg
const float LATITUDE_HERE = 48.9445;
/// Longitude value for calculating the sunset and sunrise
const float LONGITUDE_HERE = 9.3372;
/// Timezone for calculating the sunset and sunrise in hours
const float TIMEZONE_HERE_HOURS = 1.0;



// AutoUpdate-Settings
const char autoUpdatePath[] = "http://192.168.1.10/KSAutoUpdate/binaries/";
const char autoUpdateStatePath[] = "/KSAutoUpdate/api/v1/KSAutoUpdateState.json";
const char autoUpdateReadStatusPath[] = "/KSAutoUpdate/api/v1/ReadAutoUpdateStatus.php";
const char autoUpdateWriteStatusPath[] = "/KSAutoUpdate/api/v1/WriteStatus.php";


const char smtpHost[] = "smtp.gmail.com";
const uint16_t SMTP_PORT = 465;     // port 465 is not available for Outlook.com





#endif          // #define _KSCREDENTIALS_H
