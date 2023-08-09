/**
 * @file KSStatusData.cpp
 * 
 * @brief Implementation of StatusData structure and StatusDataController Class
 * 
 * @details
 * 
 * @author Joachim Kittelberger <jkittelberger@kibesoft.de>
 * @date 24.04.2022
 * @version 1.01
 *
 * @todo
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

#if defined USE_KSStatusData || defined KSLIBRARIES_USEALL     // include File in Build only if it ist defined to use it


#include "KSStatusData.h"

#include "KSAppInfo/src/KSAppInfo.h"
#include "KSRTC/src/KSRTC.h"
#include "KSUtilities/src/KSUtilities.h"
#include "KSLogger/src/KSLogger.h"



#define UPTIMEBUFFER_LEN (sizeof("dddd:hh:mm:ss"))


String KSStatusDataController::getJson() {
	if (_pStatusData) {
		updateSensorValues();

		csAccess.EnterCriticalSection();

		StaticJsonDocument<512> doc;

		tm timeInfo = { 0 };
		time_t2TimeInfo(&_pStatusData->lastReadingTime, &timeInfo);

		KSAppInfo app;
		doc["Project"] = app.getProjectName();
		doc["Version"] = app.getCurrentVersion();
		doc["DeviceID"] = app.getDeviceName();
		doc["DeviceType"] = app.getDeviceType();

		doc["IPAddress"] = app.getIPAddress();
		doc["Time"] = KSRTC::getDateTimeISO8601Formated(timeInfo);

        
		// hier den Überlauf von 32bit bei millis() mit berechnen. Wenn z.B letzte Millis größer als neue
		// Berechnung sonst falsch, wenn millies dauerhaft laufen
		// hier muss ein millis-Offset mutlipliziert werden, mit der Anzahl Überläufe
		uint32_t currentMillis = millis();
		if (_lastMillisUptime > currentMillis) {	// test, if we have an overflow of currentMillis
			_millisOverflow++;
		}
		_lastMillisUptime = currentMillis;

		// add current time to deepsleeptime
		uint64_t uptime = _pStatusData->upTimeMS + currentMillis + uint64_t(_millisOverflow * UINT32_MAX);
		
		doc["Uptime"] = formatUptime(uptime);
		doc["UptimeSeconds"] = (uint32_t)(uptime / 1000);
		doc["LastReset"] = getLastResetDateTime();;

		if (_pIKSBatteryPower) {
//			doc["Voltage"] = String(_pStatusData->batteryLevel, 3);										// string, runde auf 3 Nachkommastellen
//			doc["BatteryLevelPercent"] = String(_pStatusData->batteryLevelPercent, 1);					// string, runde auf 1 Nachkommastellen
			doc["Voltage"] = (float)((int)(_pStatusData->batteryLevel * 1000)) / 1000.0;							// float, runde auf 3 Nachkommastellen
			doc["BatteryLevelPercent"] = (float)((int)(_pStatusData->batteryLevelPercent * 10)) / 10.0;				// float runde auf 1 Nachkommastellen
		}
		if (_pIKSTemperature) {
			doc["Temperature"] = (float)((int)(_pStatusData->temperature * 100)) / 100.0;							// float runde auf 2 Nachkommastellen
		}
		if (_pIKSHumidity) {
			doc["Humidity"] = (float)((int)(_pStatusData->humidity * 100)) / 100.0;									// float runde auf 2 Nachkommastellen
		}

		// TODO über external sensor machen!
		if (_hasHWRTC) {
			doc["RTCPower"] = _pStatusData->rtcPowerOFF ? "OFF" : "ON";
		} else {
//			doc["RTCPower"] = "no RTC";
		}

		doc["mqttErrors"] = _pStatusData->mqttErrors;
		doc["smtpErrors"] = _pStatusData->smtpErrors;


		String output;
		serializeJson(doc, output);

	//	char output[1024];
	//	serializeJson(doc, output, sizeof(output));
		//LOGGER.println(output);

		csAccess.LeaveCriticalSection();

		return output;
	}
	
	return String("");
}



void KSStatusDataController::printContent() {
	LOGGER.println(getJson());
}



String KSStatusDataController::formatUptime(uint64_t timeVal) {
	char buffer[UPTIMEBUFFER_LEN];

	uint32_t uptimeSeconds = (uint32_t)(timeVal / 1000);
	uint32_t uptimeMinutes = uptimeSeconds / 60;
	uint32_t uptimeHours = uptimeMinutes / 60;
	uint32_t uptimeDays = uptimeHours / 24;
	uint32_t seconds = uptimeSeconds % 60;
	uint32_t minutes = uptimeMinutes % 60;
	uint32_t hours = uptimeHours % 24;
	uint32_t days = uptimeDays;

	snprintf(buffer, sizeof(buffer), PSTR("%04d:%02d:%02d:%02d"), days, hours, minutes, seconds);
	return String(buffer);
}



String KSStatusDataController::getLastResetDateTime() {
	tm timeInfo = { 0 };

	if (_pStatusData->lastResetDateTime == 0) {		// wenn noch nicht gesetzt. Z.B. beim ersten Mal in DeepSleep gegangen
		_pStatusData->lastResetDateTime = getCurrentTime();
	}
	time_t2TimeInfo(&_pStatusData->lastResetDateTime, &timeInfo);
	return KSRTC::getDateTimeISO8601Formated(timeInfo);
}



void KSStatusDataController::updateSensorValues() {
	csAccess.EnterCriticalSection();
	
    // set callback functions for e.g. switch on/of sensor power
	if (onBeforeUpdateSensorValuesListener) {
		onBeforeUpdateSensorValuesListener();
	}
	
	if (_pStatusData) {

		// die Zeiten updaten...
		_pStatusData->lastReadingTime = getCurrentTime();		// set current date time of JSON creatioN as last reading time

		if (_pIKSBatteryPower) {
			_pStatusData->batteryLevel = _pIKSBatteryPower->getVoltage();
			_pStatusData->batteryLevelPercent = _pIKSBatteryPower->getBatteryPercentage();
			//_pStatusData->batteryLevelPercent = KSBatteryPower::calculateBatteryPercentage(_pStatusData->batteryLevel);
		}
		if (_pIKSTemperature) {
			_pStatusData->temperature = _pIKSTemperature->getTemperature();
		}
		if (_pIKSHumidity) {
			_pStatusData->humidity = _pIKSHumidity->getHumidity();
		}
	}
	
    // set callback functions for e.g. switch on/of sensor power
	if (onAfterUpdateSensorValuesListener) {
		onAfterUpdateSensorValuesListener();
	}

	csAccess.LeaveCriticalSection();
}

#endif		// #if defined USE_KSStatusData || defined KSLIBRARIES_USEALL