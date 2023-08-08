/**
 * @file KSMQTTConnection.cpp
 * 
 * @brief Implementation file for using KSMQTTConnection class
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

#if defined USE_KSMQTTConnection || defined KSLIBRARIES_USEALL     // include File in Build only if it ist defined to use it

#include "KSMQTTConnection.h"
#include "KSEventGroupNetwork/src/KSEventGroupNetwork.h"



KSMQTTConnection::KSMQTTConnection() {

	csClientAccess.EnterCriticalSection();
    client.setClient(espClient);

//    client.setServer(mqttServerIP, MQTT_Port);		// perhabs we have a problem with resolution of domain in WiFiClient
	IPAddress mqttIP;
	if (!mqttIP.fromString(mqttServerIP)) {
		Serial.printf("[mqtt]: Error in localIP.fromString(%s);\n", mqttServerIP);
	}
    client.setServer(mqttIP, MQTT_Port);

	client.setBufferSize(MQTT_MAX_PACKET_SIZE_KS);
	csClientAccess.LeaveCriticalSection();

//	client.setCallback(mqttCallback);			// Funktioniert nur mit statischer Funktion mqttCallback

	// init subscriptionlist
	for (int i = 0; i < MQTT_MAX_SUBSCRIPTIONS; i++) {
		_subscriptions[i] = NULL;
	}
}


KSMQTTConnection::~KSMQTTConnection() {
	// delte subscriptionlist
	for (int i = 0; i < MQTT_MAX_SUBSCRIPTIONS; i++) {
		if (_subscriptions[i] != NULL) {
			delete(_subscriptions[i]);
			_subscriptions[i] = NULL;
		}
	}
}




TaskHandle_t KSMQTTConnection::createConnection(EventGroupHandle_t *phEventGroupNetwork) {

	_phEventGroupNetwork = phEventGroupNetwork;

	int coreID = xPortGetCoreID();
	//Serial.print(F("CoreID: "));
	//Serial.println(coreID);
	
	UBaseType_t setupPriority = uxTaskPriorityGet(NULL);
	//Serial.print(F("setup: priority = "));
	//Serial.println(setupPriority);

	xTaskCreatePinnedToCore(
    	[](void* context){ static_cast<KSMQTTConnection*>(context)->tKSMQTTConnection(); },
		"tKSMQTTConnection",
		8192,
		this,
		setupPriority,
		&_htKSMQTTConnection,
		coreID
		);

	return _htKSMQTTConnection;
}



void KSMQTTConnection::tKSMQTTConnection()
{
    // Warten auf WIFI-Initialisierung
    if (_phEventGroupNetwork && (*_phEventGroupNetwork != NULL)) {
        if ((xEventGroupGetBits(*_phEventGroupNetwork) & EG_NETWORK_INITIALIZED) == 0) {
            //Serial.println(F("[mqtt] Wating for Event EG_NETWORK_INITIALIZED for 5 seconds"));
          	EventBits_t eventGroupValue;
            eventGroupValue = xEventGroupWaitBits(*_phEventGroupNetwork, EG_NETWORK_INITIALIZED, pdFALSE, pdTRUE, pdMS_TO_TICKS(5000));
            //Serial.println(F("[mqtt] Event EG_NETWORK_INITIALIZED set"));
        }
    } else {
        Serial.println(F("[mqtt] no EventGroup for EG_NETWORK_INITIALIZED. Waiting for 5 seconds"));
        vTaskDelay(pdMS_TO_TICKS(5000));    // let time for Wifi to initialie
    }


	// connect the callback function. Wird benötigt, damit eine member-function als callback verwendet werden kann. Sonst muss diese Static definiert sein.
    std::function<void(char*, uint8_t*, unsigned int)> memPtr = NULL;
	memPtr = std::bind(&KSMQTTConnection::mqttCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	client.setCallback(memPtr);


	// main loop mqtt
    for (;;) {

		csClientAccess.EnterCriticalSection();
		bool bConnected = client.connected();
		csClientAccess.LeaveCriticalSection();

		if (!bConnected) {
			this->reconnect();
		}
		csClientAccess.EnterCriticalSection();
		client.loop();
		csClientAccess.LeaveCriticalSection();

		vTaskDelay(pdMS_TO_TICKS(500));
	}
}



void KSMQTTConnection::reconnect() {

	csClientAccess.EnterCriticalSection();
	bool bConnected = client.connected();
	csClientAccess.LeaveCriticalSection();

    while (!bConnected && counter < 3) {    // TODO Abbruch nach 3 Versuchen

		// Warte auf Wifi-Verbindung und Initialisierung:
		// Wenn Connection-Bit noch nicht gesetzt, dann das erste Mal warten.
		if (_phEventGroupNetwork && (*_phEventGroupNetwork != NULL)) {

			// Reset Connected Event
			if ((xEventGroupGetBits(*_phEventGroupNetwork) & EG_NETWORK_MQTT_CONNECTED) == EG_NETWORK_MQTT_CONNECTED) {
				xEventGroupClearBits(*_phEventGroupNetwork, EG_NETWORK_MQTT_CONNECTED);
			}
			// Waiting for Wifi-Connection
			if ((xEventGroupGetBits(*_phEventGroupNetwork) & EG_NETWORK_CONNECTED) == 0) {
				Serial.println(F("[mqtt] Wating for Event EG_NETWORK_CONNECTED"));
	
				// Reset Connecting Event
				if (_phEventGroupNetwork && (*_phEventGroupNetwork != NULL)) {
					if ((xEventGroupGetBits(*_phEventGroupNetwork) & EG_NETWORK_MQTT_CONNECTING) == EG_NETWORK_MQTT_CONNECTING) {
						xEventGroupClearBits(*_phEventGroupNetwork, EG_NETWORK_MQTT_CONNECTING);
					}
				}
	
				EventBits_t eventGroupValue;
				eventGroupValue = xEventGroupWaitBits(*_phEventGroupNetwork, (EG_NETWORK_INITIALIZED | EG_NETWORK_CONNECTED), pdFALSE, pdTRUE, portMAX_DELAY);
				Serial.println(F("[mqtt] Event EG_NETWORK_CONNECTED set"));
			}

			// Set Connecting Event
			if ((xEventGroupGetBits(*_phEventGroupNetwork) & EG_NETWORK_MQTT_CONNECTING) == 0) {
				xEventGroupSetBits(*_phEventGroupNetwork, EG_NETWORK_MQTT_CONNECTING);
			}
		}

        Serial.print(F("[mqtt] Starte MQTT-Verbindung..."));
		csClientAccess.EnterCriticalSection();
		bConnected = client.connect(mqttClientID, mqttUserName, mqttUserPassword);
		csClientAccess.LeaveCriticalSection();

        if (bConnected) {
            Serial.println(F("verbunden"));
//            client.publish("home/test/live", "I\'m alive!");
//            client.subscribe("home/test/live");

			// Reset Connecting Event
			// Set Connected Event
			if (_phEventGroupNetwork && (*_phEventGroupNetwork != NULL)) {
				if ((xEventGroupGetBits(*_phEventGroupNetwork) & EG_NETWORK_MQTT_CONNECTING) == EG_NETWORK_MQTT_CONNECTING) {
					xEventGroupClearBits(*_phEventGroupNetwork, EG_NETWORK_MQTT_CONNECTING);
				}
				if ((xEventGroupGetBits(*_phEventGroupNetwork) & EG_NETWORK_MQTT_CONNECTED) == 0) {
					xEventGroupSetBits(*_phEventGroupNetwork, EG_NETWORK_MQTT_CONNECTED);
				}
			}

			// unsubscribe old Subscriptions
			// check if we have subscriptions and subscribe them
			for (int i = 0; i < MQTT_MAX_SUBSCRIPTIONS; i++) {
				if (_subscriptions[i] != NULL) {
					if (_subscriptions[i]->_isSubscribed) {
						csClientAccess.EnterCriticalSection();
						if (client.unsubscribe(_subscriptions[i]->getTopicName()))
							_subscriptions[i]->_isSubscribed = false;
						csClientAccess.LeaveCriticalSection();
					}
				}
			}

			// check if we have subscriptions and subscribe them
			for (int i = 0; i < MQTT_MAX_SUBSCRIPTIONS; i++) {
				if (_subscriptions[i] != NULL) {
					csClientAccess.EnterCriticalSection();
					if (client.subscribe(_subscriptions[i]->getTopicName()))
						_subscriptions[i]->_isSubscribed = true;
					csClientAccess.LeaveCriticalSection();
				}
			}

        } else {
            Serial.print(F("Fehler, rc="));
			csClientAccess.EnterCriticalSection();
            Serial.print(client.state());
			csClientAccess.LeaveCriticalSection();
            Serial.println(F(" versuche es in 30 Sekunden erneut"));
            counter++;
			//Serial.print(".");
            vTaskDelay(pdMS_TO_TICKS(30000));
        }
    }
    counter = 0;
}



bool KSMQTTConnection::isConnected() {
	csClientAccess.EnterCriticalSection();
	bool bConnected = client.connected();
	csClientAccess.LeaveCriticalSection();

	return bConnected;
}




boolean KSMQTTConnection::publish(const char* topic, const char* payload)
{
	csClientAccess.EnterCriticalSection();
	bool bRetVal = client.publish(topic, payload);
	csClientAccess.LeaveCriticalSection();

	return bRetVal;
}



bool KSMQTTConnection::setOnSubscribedTopicListener(const char* topic, void (*callback)(char* topic, byte* payload, unsigned int length)) {

	// check if we have a free subscription and add the given one
	for (int i = 0; i < MQTT_MAX_SUBSCRIPTIONS; i++) {
		if (_subscriptions[i] == NULL) {
			// insert in subscriber-List of topics
			_subscriptions[i] = new KSMQTTSubscriptionTopic(topic, callback);
			// if we have a connection, subscribe
			if ((xEventGroupGetBits(*_phEventGroupNetwork) & EG_NETWORK_MQTT_CONNECTED) == EG_NETWORK_MQTT_CONNECTED) {
				csClientAccess.EnterCriticalSection();
				if (client.subscribe(topic))
					_subscriptions[i]->_isSubscribed = true;
				csClientAccess.LeaveCriticalSection();
			}
			return true;
		}
	}

	// no free subscription
	return false;
}



// remove Subscrption from list and delete this subscription
bool KSMQTTConnection::removeOnSubscribeTopicListener(const char* topic) {
	bool bFound = false;

	// check if we have a free subscription
	for (int i = 0; i < MQTT_MAX_SUBSCRIPTIONS; i++) {
		if (_subscriptions[i] != NULL) {
			if (strcmp(topic, _subscriptions[i]->getTopicName()) == 0) {
				csClientAccess.EnterCriticalSection();
				client.unsubscribe(_subscriptions[i]->getTopicName());
				_subscriptions[i]->_isSubscribed = false;
				csClientAccess.LeaveCriticalSection();

				delete(_subscriptions[i]);
				_subscriptions[i] = NULL;
				bFound = true;
			}
		}
	}
	return bFound;
}



// callback for PubSubClient
void KSMQTTConnection::mqttCallback(char* topic, byte* payload, unsigned int length) {
//    Serial.print(F("Nachricht eingegangen ["));
//    Serial.print(topic);
//    Serial.print(F("] "));
//    for (int i = 0; i < length; i++) {
//        Serial.print((char)payload[i]);
//    }
//    Serial.println();

	// TODO: benötigen wir das wirklich???
//	if (strcmp(topic, mqttTopicLive) == 0) {
		// its this topic
//	}


	// durch ganze Liste der Subscriber gehen und Topic vergleichen.
	// check if we have a subscription for this topic
	for (int i = 0; i < MQTT_MAX_SUBSCRIPTIONS; i++) {
		if (_subscriptions[i] != NULL) {
			if (strcmp(topic, _subscriptions[i]->getTopicName()) == 0) {
				if (_subscriptions[i]->onSubscribedTopicListener != NULL) {
					_subscriptions[i]->onSubscribedTopicListener(topic, payload, length);
				}
			}
		}
	}
}



// close the connection and flush pending publications
// https://github.com/knolleary/pubsubclient/issues/452
// trigger disconnection from MQTT broker
void KSMQTTConnection::closeOpenConnection() {

	csClientAccess.EnterCriticalSection();
	client.disconnect(); 
	espClient.flush();
	csClientAccess.LeaveCriticalSection();
	// delay(10);
	// wait until connection is closed completely

	csClientAccess.EnterCriticalSection();
	int state = client.state();
	csClientAccess.LeaveCriticalSection();
	while (state != MQTT_DISCONNECTED) {  
        vTaskDelay(pdMS_TO_TICKS(10));
		csClientAccess.EnterCriticalSection();
		state = client.state();
		csClientAccess.LeaveCriticalSection();
	}
}



bool KSMQTTConnection::waitForInit(TickType_t xTicksToWait) {

	if (_phEventGroupNetwork && (*_phEventGroupNetwork != NULL)) {

		// hier auf mqtt warten
		EventBits_t eventGroupValue;
		eventGroupValue = xEventGroupWaitBits(*_phEventGroupNetwork, EG_NETWORK_MQTT_CONNECTED, pdFALSE, pdTRUE, xTicksToWait);
		// wenn Timeout, dann in DeepSleep gehen
		if ((eventGroupValue & EG_NETWORK_MQTT_CONNECTED) != EG_NETWORK_MQTT_CONNECTED) {
    		//Serial.printf("No Connection to MQTT.\n");
			return false;
		}
		//Serial.println("Connected to mqtt-server.");
		return true;
	}

	return false;
}


#endif		// #if defined USE_KSMQTTConnection || defined KSLIBRARIES_USEALL