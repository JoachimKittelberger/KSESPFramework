/**
 * @file KSTelnetServer.cpp
 * 
 * @brief Implementation of KSTelnetServer class
 * 
 * @details
 * 
 * @see
 * 
 * @author Joachim Kittelberger <jkittelberger@kibesoft.de>
 * @date 24.04.2022
 * @version 1.01
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

#if defined USE_KSTelnetServer || defined KSLIBRARIES_USEALL     // include File in Build only if it ist defined to use it


#include "KSTelnetServer.h"
#include "KSEventGroupNetwork/src/KSEventGroupNetwork.h"



KSTelnetServer::KSTelnetServer(uint16_t port) : TelnetStreamClass(port) {
    onReadCommandListener = NULL;
    onReadSetCommandListener = NULL;
}


KSTelnetServer::~KSTelnetServer() {
}


TaskHandle_t KSTelnetServer::createConnection(EventGroupHandle_t *phEventGroupNetwork) {

	_phEventGroupNetwork = phEventGroupNetwork;
 
	int coreID = xPortGetCoreID();
	//Serial.print(F("CoreID: "));
	//Serial.println(coreID);
	
	UBaseType_t setupPriority = uxTaskPriorityGet(NULL);
	//Serial.print(F("setup: priority = "));
	//Serial.println(setupPriority);

	xTaskCreatePinnedToCore(
    	[](void* context){ static_cast<KSTelnetServer*>(context)->tKSTelnetServer(); },
		"tKSTelnetServer",
		8192,
		this,
		setupPriority,
		&_htKSTelnetServer,
		coreID
		);

	return _htKSTelnetServer;
}



void KSTelnetServer::tKSTelnetServer()
{
    // Warte auf Wifi-Verbindung und Initialisierung:
    // Wenn Connection-Bit noch nicht gesetzt, dann das erste Mal warten.
    if (_phEventGroupNetwork && (*_phEventGroupNetwork != NULL)) {
        if ((xEventGroupGetBits(*_phEventGroupNetwork) & EG_NETWORK_CONNECTED) == 0) {
            //Serial.println(F("[telnet] Wating for Event EG_NETWORK_CONNECTED"));
            EventBits_t eventGroupValue;
            eventGroupValue = xEventGroupWaitBits(*_phEventGroupNetwork, (EG_NETWORK_INITIALIZED | EG_NETWORK_CONNECTED), pdFALSE, pdTRUE, portMAX_DELAY);
            //Serial.println(F("[telnet] Event EG_NETWORK_CONNECTED set"));
        }
    }
            
    this->begin();

	
	// main loop telnet
    for (;;) {

        // Warte auf Wifi-Verbindung und Initialisierung:
        // Wenn Connection-Bit noch nicht gesetzt, dann das erste Mal warten.
        if (_phEventGroupNetwork && (*_phEventGroupNetwork != NULL)) {
            if ((xEventGroupGetBits(*_phEventGroupNetwork) & EG_NETWORK_CONNECTED) == 0) {
                //Serial.println(F("[telnet] Wating for Event EG_NETWORK_CONNECTED"));
                EventBits_t eventGroupValue;
                eventGroupValue = xEventGroupWaitBits(*_phEventGroupNetwork, (EG_NETWORK_INITIALIZED | EG_NETWORK_CONNECTED), pdFALSE, pdTRUE, portMAX_DELAY);
                //Serial.println(F("[telnet] Event EG_NETWORK_CONNECTED set"));
            }
        }

        // hier die Comands einlesen udn Event-Routine aufrufen
        // TODO: hier komplettes Kommando bis /R/N einlesen


        if (this->available()) {

            char ch = this->read();
            
            // Handle the commands in case of initialization
            if (ch == TELNET_IAC) {     // it is a command with 1-3 bytes
                char command = 0;
                if (!readNextCharWithTimeout(command, 20)) continue;

                // Test if we have a 3 Byte command
                if ((command == TELNET_WILL) || (command == TELNET_WONT) || (command == TELNET_DO) || (command == TELNET_DONT)) {
                    char cmdData = 0;
                    if (!readNextCharWithTimeout(cmdData, 20)) continue;
                    this->printf("[telnet] TELNET_CMD: %d (%d)\n", (int)command, (int)cmdData);
                } else {
                    this->printf("[telnet] TELNET_CMD: %d\n", (int)command);
                }
         		vTaskDelay(pdMS_TO_TICKS(10));
                continue;
            }


            // read in the application comands
            int currentChar = 0;
            _szTelnetCmd[currentChar] = 0;

            while ((ch != TELNET_CR && ch != TELNET_LF)) {
                _szTelnetCmd[currentChar++] = ch;
                
                // maximum allowed length
                if (currentChar >= TELNET_CMD_LENGTH) break;

                if (!readNextCharWithTimeout(ch, 20)) {
                    // Timeout -> exit while loop
                    break;
                }
            }
            
            // remove the CR, LF from queue
            while (this->available()) {
                ch = this->peek();
                if ((ch == TELNET_CR || ch == TELNET_LF)) {
                    ch = this->read();
                } else {
                    break;
                }
            }
            _szTelnetCmd[currentChar] = 0;
            //this->printf("[telnet] CMD: %s\n", _szTelnetCmd);

            // handle the commands
            if (_handleDefaultCommands) {
               	if (strcasecmp(_szTelnetCmd, "Restart") == 0) {
                    //Serial.println("[telnet] [R] Restart ESP");
                    this->println("Restarting ...");
                    this->flush();
                    this->stop();
            		vTaskDelay(pdMS_TO_TICKS(100));
                    ESP.restart();
                }
               	else if (strcasecmp(_szTelnetCmd, "Quit") == 0) {
                    //Serial.println("[telnet] [Q] Quit");
                    this->println("bye bye");
                    this->flush();
                    this->stop();
                    //this->disconnectClient();
                }
               	else if (strcasecmp(_szTelnetCmd, "StartCyclic") == 0) {
                    //Serial.println("[telnet] [G] Go send cyclic Data");
                    this->println("[telnet] Start sending cyclic data");
                    _sendCyclicData = true;
                }
               	else if (strcasecmp(_szTelnetCmd, "StopCyclic") == 0) {
                    //Serial.println("[telnet] [S] Stop send cyclic Data");
                    this->println("[telnet] Stop sending cyclic data");
                    _sendCyclicData = false;
                }

//               	else if (strcmp(_szTelnetCmd, "Info") == 0) {
//                    Serial.println("Telnet: [I] Get Info");
//                    this->printf("Project: %s Version: %s\n", PROJECT_NAME, SW_VERSION);
//               }
                else {
                    // test if we have a set command
                    // we have a set command in the following format: set param=value
                    String cmd = _szTelnetCmd;
                    if (cmd.substring(0, 4).equalsIgnoreCase("set ") && onReadSetCommandListener) {
                        //this->println("set command");
                        String paramVal = cmd.substring(4);
                        int indexValue = paramVal.indexOf("=");
                        if (indexValue >= 0) {
                            String param = paramVal.substring(0, indexValue);
                            param.trim();
                            String value = paramVal.substring(indexValue + 1);
                            value.trim();
                            onReadSetCommandListener((char*)param.c_str(), (char*)value.c_str());
                        }
                    } else {

                        // handle it in onReadCommandListener
                        if (onReadCommandListener != NULL) {
                            onReadCommandListener(_szTelnetCmd);
                        }
                    }
                }
            } else {            // if _handleDefaultCommands == false, do all in onReadCommandListener
                if (onReadCommandListener != NULL) {
                    onReadCommandListener(_szTelnetCmd);
                }
            }
        }

		vTaskDelay(pdMS_TO_TICKS(200));
	}
}


// reads the next char from the telnet channel with timeout
bool KSTelnetServer::readNextCharWithTimeout(char& ch, int timeout) {
    unsigned long startMillis = millis();

    while (millis() - startMillis < timeout) {
        if (this->available()) {
            ch = this->read();
            return true;
        }
		vTaskDelay(pdMS_TO_TICKS(1));
    }
    ch = 0;
    return false;   
}


KSTelnetServer Telnet(23);


#endif      // #if defined USE_KSTelnetServer || defined KSLIBRARIES_USEALL