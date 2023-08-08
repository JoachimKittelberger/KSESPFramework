/**
 * @file KSStatusData.h
 * 
 * @brief Definition of StatusData structure and StatusDataController Class
 * 
 * @details
 *   Definition of structure to store in RTC_MEMORY (RTC_DATA_ATTR)
 *   Class uses the struct and is not located in RTC_MEMORY
 *   Wenn Klasse in RTC_DATA_ATTR, dann darf kein Constructor existieren und das Objekt muss als ganzes in RTC_DATA_ATTR gelegt werden.
 *   muss dann als struct angelegt werden und nicht als Klasse
 *   es dürfen auch im der Struktur keine Objekte verwendet werden (String, ...)
 * 
 * @see
 *   - https://github.com/bblanchon/ArduinoJson.git @ ^6.21.3
 * 
 * @author Joachim Kittelberger <jkittelberger@kibesoft.de>
 * @date 24.04.2022
 * @version 1.01
 *
 * @todo
 *  - Generische Klasse daraus machen, die dann alle Daten selber sammelt und eine mqtt-Send-Funktion hat
 *  - Parametrieren, welche Daten gesendet werden sollen und welche nicht. So dass nicht immer alle bechrieben werden müssen
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

#include <ArduinoJson.h>

#include "KSInterfaces/src/KSInterfaces.h"
#include "KSCriticalSection/src/KSCriticalSection.h"


// Wenn Klasse in RTC_DATA_ATTR, dann darf kein Constructor existieren und das Objekt muss als ganzes in RTC_DATA_ATTR gelegt werden.
// muss dann als struct angelegt werden und nicht als Klasse
// es dürfen auch im der Struktur keine Objekte verwendet werden (String, ...)
// Evtl. muss eine zweite Klasse erstellt werden, die einen Pointer auf diese Struktur bekomme, zur weiteren Bearbeitung


// wenn wir hier Daten ändern, müssen wir den Flash wieder neu initialisieren!
struct KSStatusData {

    float batteryLevel = 0.0;
    float batteryLevelPercent = 0.0;        // Wird berechnet
    time_t lastResetDateTime = 0;           // in sec. geht nicht als struct oder char-array. Könnte auch anhand millis() und aktueller Zeit berechnet werden
    uint64_t upTimeMS = 0;                  // wird direkt berechnet (millis())
 
    float temperature = 0.0;
    float humidity = 0.0;
 
    bool rtcPowerOFF = false;               // true = RTC Battery is empty

    uint32_t mqttErrors = 0;
    uint32_t smtpErrors = 0;

 
    // current Date/Time from last reading. Im RTC memory funktionieren keine Objekte (String) und auch keine char arrays (char szTime[len])
    time_t lastReadingTime = 0;             // timestamp of last cpm/usiv reading
};



// Controller-Class for WeatherData
// not in RTC-memory but operates on WeatherData, which is located in RTC-Memore
class KSStatusDataController {
    public:
        KSStatusDataController(KSStatusData* pStatusData) {
            _pStatusData = pStatusData;
            onBeforeUpdateSensorValuesListener = NULL;
            onAfterUpdateSensorValuesListener = NULL;
        }
        String getJson();
        void updateSensorValues();            // update all external Sensor Values

        void setHWRTC(bool available = true) { _hasHWRTC = available; }
        String getLastResetDateTime();

        // add external Sensor values
        void addBattery(IKSBatteryPower* pIKSBatteryPower) { _pIKSBatteryPower = pIKSBatteryPower; }
        void addTemperatureSensor(IKSTemperature* pIKSTemperature) { _pIKSTemperature = pIKSTemperature; }
        void addHumiditySensor(IKSHumidity* pIKSHumidity) { _pIKSHumidity = pIKSHumidity; }

        // set callback functions for e.g. switch on/of sensor power
        void setOnBeforeUpdateSensorValuesListener(void (*callback)()) { onBeforeUpdateSensorValuesListener = callback; }
        void setOnAfterUpdateSensorValuesListener(void (*callback)()) { onAfterUpdateSensorValuesListener = callback; }

    protected:
        // callback functions for e.g. switch on/of sensor power
        void (*onBeforeUpdateSensorValuesListener)();
        void (*onAfterUpdateSensorValuesListener)();


    protected:
        String formatUptime(uint64_t timeVal);

        void printContent();


    private:
        KSStatusData* _pStatusData = NULL;
        bool _hasHWRTC = false;
        uint32_t _lastMillisUptime = 0;
        int _millisOverflow = 0;

        // pointer for external Sensor Values
        IKSBatteryPower* _pIKSBatteryPower = NULL;
        IKSTemperature* _pIKSTemperature = NULL;
        IKSHumidity* _pIKSHumidity = NULL;

        KSCriticalSection csAccess;			// Critical Section for access to mqtt client and synchronize calls
};

