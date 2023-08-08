/**
 * @file KSMQTTConnection.h
 * 
 * @brief Header file for using KSMQTTConnection class
 * 
 * @details 
 * 
 * @see
 *   - used library: https://github.com/knolleary/pubsubclient.git
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

#ifndef _KSMQTTCONNECTION_H
#define _KSMQTTCONNECTION_H


#include <WiFi.h>
#include <PubSubClient.h>

#include "KSCriticalSection/src/KSCriticalSection.h"


#define MQTT_MAX_PACKET_SIZE_KS 512        // must be set in client->setBufferSize();

#ifndef MQTT_MAX_SUBSCRIPTIONS
#define MQTT_MAX_SUBSCRIPTIONS 5
#endif


#define MQTT_Port 1883
//#define MQTT_Port 8883  // for TLS


class KSMQTTSubscriptionTopic;

class KSMQTTConnection {
    public:
        KSMQTTConnection();
        ~KSMQTTConnection();

        TaskHandle_t createConnection(EventGroupHandle_t *phEventGroupNetwork = NULL);

        boolean publish(const char* topic, const char* payload);
        bool setOnSubscribedTopicListener(const char* topic, void (*callback)(char* topic, byte* payload, unsigned int length));
        bool removeOnSubscribeTopicListener(const char* topic);

        void closeOpenConnection();
        bool isConnected();
        
        bool waitForInit(TickType_t xTicksToWait = portMAX_DELAY);

    protected:

    private:
        void tKSMQTTConnection();

        void reconnect();
        void mqttCallback(char* topic, byte* payload, unsigned int length);

        WiFiClient espClient;
        PubSubClient client;

        KSCriticalSection csClientAccess;			// Critical Section for access to mqtt client and synchronize calls
    
        int counter = 0;

        KSMQTTSubscriptionTopic* _subscriptions[MQTT_MAX_SUBSCRIPTIONS];

        TaskHandle_t _htKSMQTTConnection;
        EventGroupHandle_t *_phEventGroupNetwork = NULL;
};



class KSMQTTSubscriptionTopic {
    friend class KSMQTTConnection;
    
    public:
        KSMQTTSubscriptionTopic(const char* topic, void (*callback)(char* topic, byte* payload, unsigned int length)) {
            //Serial.printf("Constructor KSMQTTSubscriptionTopic topic: %s\n", topic);
            _topic = strdup(topic);
            onSubscribedTopicListener = callback;
        };
        virtual ~KSMQTTSubscriptionTopic() {
            //Serial.printf("Destructor KSMQTTSubscriptionTopic topic: %s\n", _topic);
            if (_topic != NULL)
                free(_topic);
        };

    protected:
        void (*onSubscribedTopicListener)(char* topic, byte* payload, unsigned int length);
        char* getTopicName() { return _topic; };

    private:
        char* _topic = NULL;
        bool _isSubscribed = false;
};





#endif // #define _KSMQTTCONNECTION_H