/**
 * @file KSESPFramework.h
 * 
 * @brief Header File for Framework KSESPFramework
 * 
 * @details
 * 
 * @see https://github.com/JoachimKittelberger/KSESPFramework
 * 
 * @author Joachim Kittelberger <jkittelberger@kibesoft.de>
 * @date 03.08.2023
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

#ifndef _KSESPFRAMEWORK_H
#define _KSESPFRAMEWORK_H

//#define KSLIBRARIES_USEALL 1
#ifdef KSLIBRARIES_USEALL
    #define USE_KSAppInfo 1
    #define USE_KSAutoUpdate 1
    #define USE_KSAVMConnection 1
    #define USE_KSBattery 1
    #define USE_KSBME280 1
    #define USE_KSBME280 1
    #define USE_KSBME680BSec 1
    #define USE_KSButton 1
    #define USE_KSCCS811 1
    #define USE_KSCriticalSection 1
    #define USE_KSDeepSleep 1
    #define USE_KSDHT22 1
    #define USE_KSDisplay 1
    #define USE_KSEventGroupDisplay 1
    #define USE_KSEventGroupNetwork 1
    #define USE_KSFileSystem 1
    #define USE_KSFTPServer 1
    #define USE_KSGY302 1
    #define USE_KSHCSR04 1
    #define USE_KSHomeKit 1
    #define USE_KSInstaModul 1
    //#define USE_KSInterfaces 1
    #define USE_KSJET32Connection 1
    #define USE_KSLEDs 1
    #define USE_KSLiquidCrystal_I2C 1
    #define USE_KSLogger 1
    #define USE_KSMHZ19b 1
    #define USE_KSMQ135 1
    #define USE_KSMQTTConnection 1
    #define USE_KSMutex 1
    #define USE_KSNTPClient 1
    #define USE_KSNTPClientLight 1
    #define USE_KSOTA 1
    #define USE_KSPrefereces 1
    #define UES_KSResetController 1
    #define USE_KSRTC 1
    #define USE_KSShelly 1
    #define USE_KSStatusData 1
    #define USE_KSStratoMailClient 1
    #define USE_KSTelnetServer 1
    #define USE_KSTelnetServer2 1
    #define USE_KSTEMT6000 1
    #define USE_KSULPWakeup 1
    #define USE_KSUtilities 1
    #define USE_KSWiFiConnection 1
    #define USE_KSWS2812B 1

#endif      // KSLIBRARIES_USEALL




#ifdef USE_KSAppInfo
    #include "../src/KSAppInfo/src/KSAppInfo.h"
#endif

#ifdef USE_KSAutoUpdate
    #include "../src/KSAutoUpdate/src/KSAutoUpdate.h"
#endif

#ifdef USE_KSAVMConnection
    #include "../src/KSAVMConnection/src/KSAVMConnection.h"
#endif

#ifdef USE_KSBattery
    #include "../src/KSBattery/src/KSBattery.h"
#endif

#ifdef USE_KSBME280
    #include "../src/KSBME280/src/KSBME280.h"
#endif

#ifdef USE_KSBME680
    #include "../src/KSBME680/src/KSBME680.h"
#endif

#ifdef USE_KSBME680BSec
    #include "../src/KSBME680BSec/src/KSBME680BSec.h"
#endif

#ifdef USE_KSButton
    #include "../src/KSButton/src/KSButton.h"
#endif

#ifdef USE_KSCCS811
    #include "../src/KSCCS811/src/KSCCS811.h"
#endif

#ifdef USE_KSCriticalSection
    #include "../src/KSCriticalSection/src/KSCriticalSection.h"
#endif

#ifdef USE_KSDeepSleep
    #include "../src/KSDeepSleep/src/KSDeepSleep.h"
#endif

#ifdef USE_KSDHT22
    #include "../src/KSDHT22/src/KSDHT22.h"
#endif

#ifdef USE_KSDisplay
    #include "../src/KSDisplay/src/KSDisplay.h"
#endif

#ifdef USE_KSEventGroupDisplay
    #include "../src/KSEventGroupDisplay/src/KSEventGroupDisplay.h"
#endif

#ifdef USE_KSEventGroupNetwork
    #include "../src/KSEventGroupNetwork/src/KSEventGroupNetwork.h"
#endif

#ifdef USE_KSFileSystem
    #include "../src/KSFileSystem/src/KSFileSystem.h"
#endif

#ifdef USE_KSFTPServer
    #include "../src/KSFTPServer/src/KSFTPServer.h"
#endif

#ifdef USE_KSGY302
    #include "../src/KSGY302/src/KSGY302.h"
#endif

#ifdef USE_KSHCSR04
    #include "../src/KSHCSR04/src/KSHCSR04.h"
#endif

#ifdef USE_KSHomeKit
    #include "../src/KSHomeKit/src/KSHomeKit.h"
#endif

#ifdef USE_KSInstaModul
    #include "../src/KSInstaModul/src/KSInstaModul.h"
#endif

// should be included from classes which use this interaface
//#ifdef USE_KSInterfaces
//    #include "../src/KSInterfaces/src/KSInterfaces.h"
//#endif

#ifdef USE_KSJET32Connection
    #include "../src/KSJET32Connection/src/KSJet32Connection.h"
#endif

#ifdef USE_KSLEDs
    #include "../src/KSLEDs/src/KSLEDs.h"
#endif

#ifdef USE_KSLiquidCrystal_I2C
    #include "../src/KSLiquidCrystal_I2C/src/KSLiquidCrystal_I2C.h"
#endif

#ifdef USE_KSLogger
    #include "../src/KSLogger/src/KSLogger.h"
#endif

#ifdef USE_KSMHZ19b
    #include "../src/KSMHZ19b/src/KSMHZ19b.h"
#endif

#ifdef USE_KSMQ135
    #include "../src/KSMQ135/src/KSMQ135.h"
#endif

#ifdef USE_KSMQTTConnection
    #include "../src/KSMQTTConnection/src/KSMQTTConnection.h"
#endif

#ifdef USE_KSMutex
    #include "../src/KSMutex/src/KSMutex.h"
#endif

#ifdef USE_KSNTPClient
    #include "../src/KSNTPClient/src/KSNTPClient.h"
#endif

#ifdef USE_KSNTPClientLight
    #include "../src/KSNTPClientLight/src/KSNTPClientLight.h"
#endif

#ifdef USE_KSOTA
    #include "../src/KSOTA/src/KSOTA.h"
#endif

#ifdef USE_KSPreferences
    #include "../src/KSPreferences/src/KSPreferences.h"
#endif

#ifdef UES_KSResetController
    #include "../src/KSResetController/src/KSResetController.h"
#endif

#ifdef USE_KSRTC
    #include "../src/KSRTC/src/KSRTC.h"
#endif

#ifdef USE_KSShelly
    #include "../src/KSShelly/src/KSShelly.h"
#endif

#ifdef USE_KSStatusData
    #include "../src/KSStatusData/src/KSStatusData.h"
#endif

#ifdef USE_KSStratoMailClient
    #include "../src/KSStratoMailClient/src/KSStratoMailClient.h"
#endif

#ifdef USE_KSTelnetServer
    #include "../src/KSTelnetServer/src/KSTelnetServer.h"
#endif

#ifdef USE_KSTelnetServer2
    #include "../src/KSTelnetServer2/src/KSTelnetServer2.h"
#endif

#ifdef USE_KSTEMT6000
    #include "../src/KSTEMT6000/src/KSTEMT6000.h"
#endif

#ifdef USE_KSULPWakeup
    #include "../src/KSULPWakeup/src/KSULPWakeup.h"
#endif

#ifdef USE_KSUtilities
    #include "../src/KSUtilities/src/KSUtilities.h"
#endif

#ifdef USE_KSWiFiConnection
    #include "../src/KSWiFiConnection/src/KSWiFiConnection.h"
#endif

#ifdef USE_KSWS2812B
    #include "../src/KSWS2812B/src/KSWS2812B.h"
#endif




/*

void testKSESPFramework();
void testKSESPFramework1();

*/

#endif      // #ifndef _KSESPFRAMEWORK_H



