/**
 * @file KSAVMConnection.h
 * 
 * @brief Header for KSAVMConnection class
 * 
 * @details handles communication to fritz.box homeautomation-switches
 * 
 * @see
 *   Used Library for communication with fritzbox:
 *     https://github.com/Aypac/Arduino-TR-064-SOAP-Library.git
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

//#pragma once
#ifndef _KSAVMCONNECTION_H
#define _KSAVMCONNECTION_H

#include "tr064.h"
#include "TR064Helper.h"

/*

Start at https://192.168.1.1:49443/tr64desc.xml (assuming the default IP for your fbox)
to find an overview over the services available on your fritz!box API. Most browsers should display a XML-structure.
Then go to the service-element containing the serviceType-tag with a content that sound like what you want.
I.e. 'urn:dslforum-org:service:WANDSLInterfaceConfig:1'.
Next, check the value of the associated SCPDURL-tag. I.e. '/wandslifconfigSCPD.xml',
so if you head over to https://192.168.1.1:49443/wandslifconfigSCPD.xml you see what actions are available on this service.
You see for instance, that in this case there is a service called 'GetInfo', so I know I could call this service with

Beispiel für communication mit Fritzbox
String params[][2] = {{"argumentName1","value1"},{"argumentName2","value2"}, ...};
a = 2 //or whatever number of arguments you are passing
String req[][2] = {{"argumentName1",""},{"argumentName2", ""},...};
b = 2 //or whatever number of arguments you are requesting
connection.action("service name", "action name", params, a, req, b);

z.B. connection.action("urn:dslforum-org:service:WANDSLInterfaceConfig:1", "GetInfo", params, a, req, b);

https://192.168.1.1:49443/x_homeautoSCPD.xml
https://192.168.1.1:49443/x_homeplugSCPD.xml

*/

const int fbPort = 49000;


typedef enum {
  OFF = 0,
  ON,
  TOGGLE,
  UNDEFINED
} SwState;

extern const char *SwStates[];    // define in cpp-File


typedef enum {
  DISCONNECTED = 0,
  REGISTERED,
  CONNECTED,
  UNKNOWN
} PresentState;

extern const char *PresentStates[];   // define in cpp-file



extern const char fbUsername[];
extern const char fbPassword[];
extern const char fbIp[];




class KSAVMConnection {

	public:
		KSAVMConnection();
		void init();

		void ensureWIFIConnection();
		bool isWIFIConnected();
		
		void SetSwitchState(String AIN, SwState state);
		bool GetSwitchState(String AIN);
    	bool IsSwitchPresent(String AIN);
    	float GetSwitchTemperature(String AIN);

		void GetDeviceInfo(String AIN);

	private:
		// TR-064 connection to fritzbox
		TR064 _fbConnection;

};




#endif    // #define _KSAVMCONNECTION_H
