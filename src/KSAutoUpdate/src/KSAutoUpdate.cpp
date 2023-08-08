/**
 * @file KSAutoUpdate.cpp
 * 
 * @brief implementation KSAutoUpdate class
 * 
 * @details handles automatic update from an update-Server
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

#if defined USE_KSAutoUpdate || defined KSLIBRARIES_USEALL    // include File in Build only if it ist defined to use it

#include "KSAutoUpdate.h"

#include <HTTPClient.h>
#include <ArduinoJson.h>

#include "KSEventGroupNetwork/src/KSEventGroupNetwork.h"


extern const char SW_VERSION[];
extern const char PROJECT_NAME[];
extern const char updateServerIP[];

extern const char autoUpdateStatePath[];
extern const char autoUpdateReadStatusPath[];
extern const char autoUpdateWriteStatusPath[];



KSAutoUpdate::KSAutoUpdate(const char *hostname)
    : /*onStartListener(NULL)
    , onEndListener(NULL)
    , onErrorListener(NULL)
    , onProgressListener(NULL)
    , */ onChangeDeepSleepListener(NULL)
{
//    if (hostname)
//   	    ArduinoOTA.setHostname(hostname);
}


KSAutoUpdate::~KSAutoUpdate() {
}



TaskHandle_t KSAutoUpdate::createConnection(EventGroupHandle_t *phEventGroupNetwork) {

	_phEventGroupNetwork = phEventGroupNetwork;

	int coreID = xPortGetCoreID();
	//Serial.print(F("CoreID: "));
	//Serial.println(coreID);
	
	UBaseType_t setupPriority = uxTaskPriorityGet(NULL);
	//Serial.print(F("setup: priority = "));
	//Serial.println(setupPriority);

	xTaskCreatePinnedToCore(
    	[](void* context){ static_cast<KSAutoUpdate*>(context)->tKSAutoUpdate(); },
		"tKSAutoUpdate",
		8192,
		this,
		setupPriority,
		&_htKSAutoUpdate,
		coreID
		);

	return _htKSAutoUpdate;
}



void KSAutoUpdate::tKSAutoUpdate()
{
    // Warte auf Wifi-Verbindung und Initialisierung:
    // Wenn Connection-Bit noch nicht gesetzt, dann das erste Mal warten.
    if (_phEventGroupNetwork && (*_phEventGroupNetwork != NULL)) {
        if ((xEventGroupGetBits(*_phEventGroupNetwork) & EG_NETWORK_CONNECTED) == 0) {
            //Serial.println(F("[ota] Wating for Event EG_NETWORK_CONNECTED"));
            EventBits_t eventGroupValue;
            eventGroupValue = xEventGroupWaitBits(*_phEventGroupNetwork, (EG_NETWORK_INITIALIZED | EG_NETWORK_CONNECTED), pdFALSE, pdTRUE, portMAX_DELAY);
            //Serial.println(F("[ota] Event EG_NETWORK_CONNECTED set"));
        }
    }
            
    setupKSAutoUpdate();
	
	// main loop ota
    for (;;) {

        // Warte auf Wifi-Verbindung und Initialisierung:
        // Wenn Connection-Bit noch nicht gesetzt, dann das erste Mal warten.
        if (_phEventGroupNetwork && (*_phEventGroupNetwork != NULL)) {
            if ((xEventGroupGetBits(*_phEventGroupNetwork) & EG_NETWORK_CONNECTED) == 0) {
                //Serial.println(F("[ota] Wating for Event EG_NETWORK_CONNECTED"));
                EventBits_t eventGroupValue;
                eventGroupValue = xEventGroupWaitBits(*_phEventGroupNetwork, (EG_NETWORK_INITIALIZED | EG_NETWORK_CONNECTED), pdFALSE, pdTRUE, portMAX_DELAY);
                //Serial.println(F("[ota] Event EG_NETWORK_CONNECTED set"));
            }
        }

        //checkForDeepSleepState();
        updateStatus();
        checkForNewUpdate();

        if (_lookupOnce) {
            // if we had a connection try again after _updateStatusInterval
    		vTaskDelay(pdMS_TO_TICKS(_updateStatusInterval * 60 * 1000));
        } else {
            // if we had no connection try again after _retryStatusIntervalNoConnection
    		vTaskDelay(pdMS_TO_TICKS(_retryStatusIntervalNoConnection * 1000));
        }
	}
}







void KSAutoUpdate::setupKSAutoUpdate() {
	// Handle ArduinoOTA
//	ArduinoOTA.setPort(8266);		// Standardport ist 8266. Kann aber abgeändert werden zur Sicherheit
	//ArduinoOTA.setHostname("TestRelais");
	//ArduinoOTA.setPassword((const char*)"admin");


//	ArduinoOTA.begin();
}



//#warning void KSAutoUpdate::checkForDeepSleepState() is deprecated. Please use void KSAutoUpdate::checkForNewUpdate().
void KSAutoUpdate::checkForDeepSleepState() {
    // JSON-Datei lesen
    // schauen, ob disable gesetzt ist

    WiFiClient wifiClient;
    HTTPClient httpclient;
    
    //String request = "http://192.168.1.10/KSAutoUpdate/KSAutoUpdateState.json";
    String request = "http://";
    request += updateServerIP;
    request += autoUpdateStatePath;
   
    //httpclient.addHeader("Content-Length", request.length());
    //httpclient.addHeader("Content-Type", "application/json");
    httpclient.begin(wifiClient, request);
    int httpResponseCode = httpclient.GET();

    if (httpResponseCode <= 0) {
        Serial.print(F("Error code: "));
        Serial.println(httpResponseCode);
        Serial.println(F("Response:"));
        Serial.println(httpclient.getString());
    } else {
        //Serial.print(F("HTTP Response code: "));
        //Serial.println(httpResponseCode);
 
        String response = httpclient.getString();
        //Serial.println(F("Response:"));
        //Serial.println(response);

        //const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(2) + 60;
        const size_t capacity = 1024;    
        DynamicJsonDocument doc(capacity);
    
        DeserializationError error = deserializeJson(doc, response);
        if (error) {
            Serial.print(F("Error: DeserializeJson(KSAutoUpdateState.json) failed: "));
            Serial.println(error.f_str());
        } else {
            for (JsonObject project : doc["Projects"].as<JsonArray>()) {

                if (project) {
                    const char* project_Project = project["Project"]; // "KHAGeigerZaehler", "KHAWeatherStation"

                    // check, if its our project
                    if (strcasecmp(project_Project, PROJECT_NAME) == 0) {

                        // handle DisableDeepSleep
                        if (project.containsKey("DisableDeepSleep")) {
                            bool project_DisableDeepSleep = project["DisableDeepSleep"]; // true, false
                            if (onChangeDeepSleepListener) {
                                onChangeDeepSleepListener(project_DisableDeepSleep);
                            }
                        }
    
                        // handle firmware update
                        if (project.containsKey("UpdateAvailable")) {
                            bool project_Update = project["UpdateAvailable"]; // true, false
                            const char* project_Version = project["Version"]; // "1.1.2", "1.1.4"
                            const char* project_binfile = project["Binfile"]; // "UpdateTestV1.02.PWD.ino.esp32.bin"

                            if (project_Update) {
                                // test if new Version > current version
                                if (_validateVersion) {
                                    
                                    if (!project_Version) {     // if no version, we can not validate -> goto next entry
                                        continue;
                                    }

                                    semver_t currentVersion = semver_t();
                                    semver_parse_version(SW_VERSION, &currentVersion);

                                    semver_t newVersion = semver_t();
                                    semver_parse_version(project_Version, &newVersion);

                                    int verCompare = semver_compare(newVersion, currentVersion);
                                    //Serial.printf("Compare Version new: %s, current: %s, Ergebnis: %d\n", SW_VERSION, project_Version, verCompare);

                                    if (verCompare < 1) {
                                        // Wenn newVersion not greater then SW_VERSION, do nothing more
                                        //Serial.printf("New Version not greater then existing Version\n");
                                        continue;
                                    }
                                }
                                
                                if (project_binfile) {
                                    if (onUpdateNewBinfileListener) {
                                        onUpdateNewBinfileListener(String(project_binfile), String(project_Version));
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    httpclient.end();
    _lookupOnce = true;

}


void KSAutoUpdate::checkForNewUpdate() {
    // WebService JSON-Rückgabe aus DB lesen
    WiFiClient wifiClient;
    HTTPClient httpclient;

    //String request = "http://192.168.1.10/KSAutoUpdate/v1/ReadAutoUpdateStatus.php";      // + PROJECT_NAME
    String request = "http://";
    request += updateServerIP;
    request += autoUpdateReadStatusPath;
    request += "?Project=";
    request += PROJECT_NAME;

    //httpclient.addHeader("Content-Length", request.length());
    //httpclient.addHeader("Content-Type", "application/json");
    httpclient.begin(wifiClient, request);
    int httpResponseCode = httpclient.GET();

    if (httpResponseCode <= 0) {
        Serial.print(F("Error code: "));
        Serial.println(httpResponseCode);
        Serial.println(F("Response:"));
        Serial.println(httpclient.getString());
    } else {
        //Serial.print(F("HTTP Response code: "));
        //Serial.println(httpResponseCode);
 
        String response = httpclient.getString();
        //Serial.print(F("Response: "));
        //Serial.println(response);


        //const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(2) + 60;
        const size_t capacity = 1024;    
        DynamicJsonDocument doc(capacity);
    
        DeserializationError error = deserializeJson(doc, response);
        if (error) {
            Serial.print(F("Error: DeserializeJson(KSAutoUpdateState.json) failed: "));
            Serial.println(error.f_str());
        } else {
            for (JsonObject project : doc["Projects"].as<JsonArray>()) {

                if (project) {
                    const char* project_Project = project["Project"]; // "KHAGeigerZaehler", "KHAWeatherStation"

                    // check, if its our project
                    if (strcasecmp(project_Project, PROJECT_NAME) == 0) {

                        // handle DisableDeepSleep
                        if (project.containsKey("DisableDeepSleep")) {
                            bool disableDeepSleep = false;      
                            const char* project_DisableDeepSleep = project["DisableDeepSleep"]; // "0", "1"
                            //if (!project_DisableDeepSleep) Serial.println("project_DisableDeepSleep = null");
                            if (project_DisableDeepSleep) {
                                if (strcasecmp(project_DisableDeepSleep, "1") == 0) {
                                //if (project_DisableDeepSleep == "1") {
                                    disableDeepSleep = true;
                                }
                                //Serial.printf("DisableDeepSleep %s\n", disableDeepSleep ? "true" : "false");
                                if (onChangeDeepSleepListener) {
                                    onChangeDeepSleepListener(disableDeepSleep);
                                }
                            }
                        }
    
                        // handle firmware update
                        if (project.containsKey("UpdateAvailable")) {
                            bool updateAvaiable = false;
                            const char* project_Update = project["UpdateAvailable"]; // "0", "1"
                            //if (!project_Update) Serial.println("project_Update = null");
                            if (project_Update) {
                                if (strcasecmp(project_Update, "1") == 0) {
                                //if (project_Update == "1") {
                                    updateAvaiable = true;
                                }
                            }
                            //Serial.printf("UpdateAvailable %s\n", updateAvaiable ? "true" : "false");

                            const char* project_Version = project["Version"]; // "1.1.2", "1.1.4"
                            //if (project_Version) Serial.printf("project_Version %s\n", project_Version);
                            const char* project_binfile = project["Binfile"]; // "UpdateTestV1.02.PWD.ino.esp32.bin"
                            //if (project_binfile) Serial.printf("project_binfile %s\n", project_binfile);

                            if (updateAvaiable) {
                                // test if new Version > current version
                                if (_validateVersion) {
                                    
                                    if (!project_Version) {     // if no version, we can not validate -> goto next entry
                                        continue;
                                    }

                                    semver_t currentVersion = semver_t();
                                    semver_parse_version(SW_VERSION, &currentVersion);

                                    semver_t newVersion = semver_t();
                                    semver_parse_version(project_Version, &newVersion);

                                    int verCompare = semver_compare(newVersion, currentVersion);
                                    //Serial.printf("Compare Version new: %s, current: %s, Ergebnis: %d\n", SW_VERSION, project_Version, verCompare);

                                    if (verCompare < 1) {
                                        // Wenn newVersion not greater then SW_VERSION, do nothing more
                                        //Serial.printf("New Version not greater then existing Version\n");
                                        continue;
                                    }
                                }
                                
                                if (project_binfile) {
                                    if (onUpdateNewBinfileListener) {
                                        onUpdateNewBinfileListener(String(project_binfile), String(project_Version));
                                    }
                                }
                            }
                            
                        }
                    }
                }
            }
        }
    }
    
    httpclient.end();
    _lookupOnce = true;

}



void KSAutoUpdate::updateStatus() {
    WiFiClient wifiClient;
    HTTPClient httpclient;
    
    String request = "http://";
    request += updateServerIP;
    request += autoUpdateWriteStatusPath;
    
    //httpclient.addHeader("Content-Length", request.length());
    httpclient.addHeader("Content-Type", "application/json");
    httpclient.begin(wifiClient, request);

	String mqttState = _pStatusDataController->getJson();
    int httpResponseCode = httpclient.POST(mqttState);
 
    if (httpResponseCode > 0) {
		String response = httpclient.getString();                       
		Serial.printf("Response Code %d\n", httpResponseCode);   
		Serial.printf("Response: %s\n", response.c_str());
    } else {
      	Serial.printf("Error (%d) occurred while sending HTTP POST: %s\n", httpResponseCode, httpclient.errorToString(httpResponseCode).c_str());
    }
    
    httpclient.end();
}

#endif      // #if defined USE_KSAutoUpdate || defined KSLIBRARIES_USEALL