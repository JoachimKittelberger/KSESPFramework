
/**
 * @file KSDisplay.h
 * 
 * @brief Header for KSDisplay class
 * 
 * @details
 * 
 * @see
 *   - Used Libraries:
 *      - https://github.com/adafruit/Adafruit-GFX-Library.git
 *      - https://github.com/davidperrenoud/Adafruit_SH1106.git
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

#ifndef _KSDISPLAY_H
#define _KSDISPLAY_H

#include <Arduino.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>

#include "KSCriticalSection/src/KSCriticalSection.h"
#include "KSRTC/src/KSRTC.h"


#define KS_OLED_SDA 21
#define KS_OLED_SCL 22


// definitions for screensaver
#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


// must be defined in KSProjectInfo.h of the ino-Projec with const char SW_VERSION[] = "1.0.0.0" ...
extern const char SW_VERSION[];         // maximum 8 char
extern const char PROJECT_NAME[];       // maximum 21 char



class KSDisplay
{
    public:
        KSDisplay();
        KSDisplay(uint8_t SDA = KS_OLED_SDA, uint8_t SCL = KS_OLED_SCL, KSCriticalSection* pcsI2C = NULL);
        ~KSDisplay();

        TaskHandle_t createConnection(EventGroupHandle_t *phEventGroupNetwork = NULL, EventGroupHandle_t *phEventGroupDisplay = NULL);

        void clearDisplay() {
 		    if (_pcsI2C) _pcsI2C->EnterCriticalSection();
           _display.clearDisplay();
           _display.display();
		    if (_pcsI2C) _pcsI2C->LeaveCriticalSection();
        }
        void showSplashscreen();
        void showScreenSaver(unsigned long duration = 10000, boolean forever = false);

        void UpdateStatusBar();
 
        void DrawScrollbar(int percent, int maxEntries);
        void testDrawAllChar(int time2waitms);
    
        void ShowCurrentPPMPage(int ppm, float temperature);
        void printTextInLine(int line, const char* pText);

        bool isInit();
        void waitForInit();


        // TODO sollte später über Klassen gemacht werden
        void SetRTC(KSRTC* pRTC) { _pRTC = pRTC; }


        // activate Display for the duration in MS. Go to Blank after this time
        void activateForDurationInMS(unsigned long duration);
    
        void setOnScreenSaverListener(void (*callback)(bool bOn)) {
            onScreenSaver = callback;
        }

        // diese 2 Funktionen gehen nur, wenn ein Reset-Pin verdrahtet ist. (Bei SH1106 aber auch ohne Reset-Pin)
        void sleepDisplay();
        void wakeupDisplay();
        bool isInDeepSleep() { return _bDisplayInDeepSleep; }


    private:
        void tKSDisplay();

        void init();
        void showLogo(unsigned long delayMS = 0);
        void showAdressInfo(unsigned long delayMS = 0);

        TaskHandle_t _htKSDisplay;
        EventGroupHandle_t *_phEventGroupDisplay = NULL;
        EventGroupHandle_t *_phEventGroupNetwork = NULL;
     
        bool _bIsInit = false;
        KSCriticalSection* _pcsI2C = NULL;


        void (*onScreenSaver)(bool bOn);



        uint8_t _SDA = KS_OLED_SDA;
        uint8_t _SCL = KS_OLED_SCL;
        Adafruit_SH1106 _display;
    
        // data for screensaver
        void initiScreenSaver();
        uint8_t _icons[NUMFLAKES][3];

        // data for blank sreensaver
	    unsigned long _millisToDeactivateScreen = ULONG_MAX;    // default ist no screensaver
        bool _bDisplayInDeepSleep = false;

        // TODO sollte später über Klassen gemacht werden
        KSRTC* _pRTC = NULL;
};

#endif // #define _KSDISPLAY_H