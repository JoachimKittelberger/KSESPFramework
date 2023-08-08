/**
 * @file KSTelnetServer2.cpp
 * 
 * @brief implementation for KSTelnetServer2 class
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

#if defined USE_KSTelnetServer2 || defined KSLIBRARIES_USEALL     // include File in Build only if it ist defined to use it


#include "KSTelnetServer2.h"
#include "KSEventGroupNetwork/src/KSEventGroupNetwork.h"



KSTelnetServer2* KSTelnetServer2::obj = NULL;


char* KSTelnetServer2::_pSW_VERSION = const_cast<char*>(SW_VERSION);
char* KSTelnetServer2::_pPROJECT_NAME = const_cast<char*>(PROJECT_NAME);


KSTelnetServer2::KSTelnetServer2(uint16_t port) {
    _port = port;
    onReadCommandListener = NULL;
    onReadSetCommandListener = NULL;
    obj = this;
}


KSTelnetServer2::~KSTelnetServer2() {
}


TaskHandle_t KSTelnetServer2::createConnection(EventGroupHandle_t *phEventGroupNetwork) {

	_phEventGroupNetwork = phEventGroupNetwork;
 
	int coreID = xPortGetCoreID();
	//Serial.print(F("CoreID: "));
	//Serial.println(coreID);
	
	UBaseType_t setupPriority = uxTaskPriorityGet(NULL);
	//Serial.print(F("setup: priority = "));
	//Serial.println(setupPriority);

	xTaskCreatePinnedToCore(
    	[](void* context){ static_cast<KSTelnetServer2*>(context)->tKSTelnetServer(); },
		"tKSTelnetServer2",
		8192,
		this,
		setupPriority,
		&_htKSTelnetServer,
		coreID
		);

	return _htKSTelnetServer;
}



void KSTelnetServer2::tKSTelnetServer()
{
    // Warte auf Wifi-Verbindung und Initialisierung:
    // Wenn Connection-Bit noch nicht gesetzt, dann das erste Mal warten.
    if (_phEventGroupNetwork && (*_phEventGroupNetwork != NULL)) {
        if ((xEventGroupGetBits(*_phEventGroupNetwork) & EG_NETWORK_CONNECTED) == 0) {
            //Serial.println(F("[telnet2] Wating for Event EG_NETWORK_CONNECTED"));
            EventBits_t eventGroupValue;
            eventGroupValue = xEventGroupWaitBits(*_phEventGroupNetwork, (EG_NETWORK_INITIALIZED | EG_NETWORK_CONNECTED), pdFALSE, pdTRUE, portMAX_DELAY);
            //Serial.println(F("[telnet2] Event EG_NETWORK_CONNECTED set"));
        }
    }
            
    setupCallbacks();

    if (!_telnet.begin()) {
        Serial.println("[telnet2] Error _telnet.begin()");
    }
	
	// main loop telnet
    for (;;) {

        // Warte auf Wifi-Verbindung und Initialisierung:
        // Wenn Connection-Bit noch nicht gesetzt, dann das erste Mal warten.
        if (_phEventGroupNetwork && (*_phEventGroupNetwork != NULL)) {
            if ((xEventGroupGetBits(*_phEventGroupNetwork) & EG_NETWORK_CONNECTED) == 0) {
                _telnet.stop();
                //Serial.println(F("[telnet2] Wating for Event EG_NETWORK_CONNECTED"));
                EventBits_t eventGroupValue;
                eventGroupValue = xEventGroupWaitBits(*_phEventGroupNetwork, (EG_NETWORK_INITIALIZED | EG_NETWORK_CONNECTED), pdFALSE, pdTRUE, portMAX_DELAY);
                //Serial.println(F("[telnet2] Event EG_NETWORK_CONNECTED set"));

                if (!_telnet.begin()) {
                    Serial.println("[telnet2] Error _telnet.begin()");
                }
            }
        }

        _telnet.loop();

		vTaskDelay(pdMS_TO_TICKS(50));
	}
}


// (optional) callback functions for telnet events
void KSTelnetServer2::onInputReceived(String str) {

    // handle the commands
    if (obj->_handleDefaultCommands) {

        //if (str == "Restart") {
        if (str.equalsIgnoreCase("Restart")) {
            //Serial.println("[telnet] [R] Restart ESP");
            obj->_telnet.println("Restarting ...");
            obj->_telnet.flush();
            vTaskDelay(pdMS_TO_TICKS(100));
            ESP.restart();
        }
        //else if (str == "Quit") {
        else if (str.equalsIgnoreCase("Quit")) {
            //Serial.println("[telnet] [Q] Quit");
            obj->_telnet.println("bye bye");
            obj->_telnet.flush();
            //obj->_telnet.stop();
            obj->_telnet.disconnectClient();
        }
        //else if (str == "StartCyclic") {
        else if (str.equalsIgnoreCase("StartCyclic")) {
            //Serial.println("[telnet] [G] Go send cyclic Data");
            obj->_telnet.println("[telnet] Start sending cyclic data");
            obj->_sendCyclicData = true;
        }
        //else if (str == "StopCyclic") {
        else if (str.equalsIgnoreCase("StopCyclic")) {
            //Serial.println("[telnet] [S] Stop send cyclic Data");
            obj->_telnet.println("[telnet] Stop sending cyclic data");
            obj->_sendCyclicData = false;
        }

//               	else if (str == "Info") == 0) {
//                    Serial.println("Telnet: [I] Get Info");
//                    this->printf("Project: %s Version: %s\n", PROJECT_NAME, SW_VERSION);
//               }
        else {

            // we have a set command in the following format: set param=value
            String cmd = str;
            if (cmd.substring(0, 4).equalsIgnoreCase("set ") && obj->onReadSetCommandListener) {
                //obj->_telnet.println("set command");
                String paramVal = cmd.substring(4);
                int indexValue = paramVal.indexOf("=");
                if (indexValue >= 0) {
                    String param = paramVal.substring(0, indexValue);
                    param.trim();
                    String value = paramVal.substring(indexValue + 1);
                    value.trim();
                    obj->onReadSetCommandListener((char*)param.c_str(), (char*)value.c_str());
                }
            } else {

                // handle it in onReadCommandListener
                if (obj->onReadCommandListener != NULL) {
                    obj->onReadCommandListener((char*)str.c_str());
                }
            }
        }
    } else {            // if _handleDefaultCommands == false, do all in onReadCommandListener
        if (obj->onReadCommandListener != NULL) {
            obj->onReadCommandListener((char*)str.c_str());
        }
    }
}







void KSTelnetServer2::onConnect(String ip) {
    Serial.print("[telnet2] ");
    Serial.print(ip);
    Serial.println(" connected");

    obj->_telnet.println("Welcome " + obj->_telnet.getIP());
    //obj->_telnet.println("(Use ^] + q  to disconnect.)");
    obj->_telnet.println("(Use Quit to disconnect.)");

    obj->_telnet.print("\nProject: ");
    if (_pPROJECT_NAME) {
        obj->_telnet.print(_pPROJECT_NAME);
    } else {
        obj->_telnet.print("Undefined"); 
    }
    obj->_telnet.print(" Version: ");
    if (_pSW_VERSION) {
        obj->_telnet.println(_pSW_VERSION);
    } else {
        obj->_telnet.println("0.0.0");
    }
}


void KSTelnetServer2::onConnectionAttempt(String ip) {
    Serial.print("[telnet2] ");
    Serial.print(ip);
    Serial.println(" tried to connected");
}


void KSTelnetServer2::onReconnect(String ip) {
    Serial.print("[telnet2] ");
    Serial.print(ip);
    Serial.println(" reconnected");
}


void KSTelnetServer2::onDisconnect(String ip) {
    Serial.print("[telnet2] ");
    Serial.print(ip);
    Serial.println(" disconnected");
}




void KSTelnetServer2::setupCallbacks() {
    // passing a lambda function
    //_telnet.onInputReceived(onInputReceived);

    //std::function<void(String)> memPtr = NULL;
    //memPtr = std::bind(&KSTelnetServer2::onInputReceived, this, std::placeholders::_1);
    //_telnet.onInputReceived(memPtr);


    // -> Geht nur mit singleton
    //_telnet.onInputReceived([&](String str) -> void {
    //    obj->onInputReceived(str);
    //});


/*  This won't work with new version from 07/2022
    // -> Geht nur mit singleton
    _telnet.onInputReceived([&](String str) -> void {
        obj->onInputReceived(str);
    });
    _telnet.onConnect([&](String ip) -> void {
        obj->onConnect(ip);
    });
    _telnet.onConnectionAttempt([&](String ip) -> void {
        obj->onConnectionAttempt(ip);
    });
    _telnet.onReconnect([&](String ip) -> void {
        obj->onReconnect(ip);
    });
    _telnet.onDisconnect([&](String ip) -> void {
        obj->onDisconnect(ip);
    });
    */

    // with static function this will work from 07/2022
    _telnet.onInputReceived(onInputReceived);
    _telnet.onConnect(onConnect);
    _telnet.onConnectionAttempt(onConnectionAttempt);
    _telnet.onReconnect(onReconnect);
    _telnet.onDisconnect(onDisconnect);


}


KSTelnetServer2 Telnet2(23);


#endif      // #if defined USE_KSTelnetServer2 || defined KSLIBRARIES_USEALL