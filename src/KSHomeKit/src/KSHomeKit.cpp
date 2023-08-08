/**
 * @file KSHomeKit.cpp
 *
 * @brief Implementation file for KSHomeKit class
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

#if defined USE_KSHomeKit || defined KSLIBRARIES_USEALL     // include File in Build only if it ist defined to use it

#include "KSHomeKit.h"

#include "devices/DEV_Identify.h"   
#include "devices/DEV_GarageDoor.h"
#include "devices/DEV_ExtSensors.h"
#include "devices/DEV_LED.h"     
#include "devices/DEV_DoorsWindows.h"
#include "devices/DEV_Blinker.h"
#include "devices/DEV_LEDwButton.h"
#include "devices/DEV_ProgButton.h"
#include "devices/DEV_Speaker.h"
#include "devices/DEV_ContactSensor.h"
#include "devices/DEV_Switch.h"
#include "devices/DEV_AVMSwitch.h"
//#include "devices/DEV_Sensors.h"



KSHomeKit::KSHomeKit(const char* hostname, const char* sketchVersion) {
  onSetupHomeSpanListener = NULL;
  _hostname = hostname;
  _sketchVersion = sketchVersion;
}


KSHomeKit::~KSHomeKit() {
}


TaskHandle_t KSHomeKit::createHomeKit() {

	int coreID = xPortGetCoreID();
	//Serial.print(F("CoreID: "));
	//Serial.println(coreID);
	
	UBaseType_t setupPriority = uxTaskPriorityGet(NULL);
	//Serial.print(F("setup: priority = "));
	//Serial.println(setupPriority);

	xTaskCreatePinnedToCore(
    [](void* context){ static_cast<KSHomeKit*>(context)->tKSHomeKit(); },
		"tKSHomeKit",
		16384,
    //8192,
		this,
		setupPriority,
		&_htKSHomeKit,
		coreID
		);

	return _htKSHomeKit;
}



void KSHomeKit::setStaticConfig(const char* pIP, const char* pGateway, const char* pSubnet, const char* pDns) {
    if (pIP && pGateway && pSubnet && pDns) {
        _staticIP.fromString(pIP);
        _staticGateway.fromString(pGateway);
        _staticSubnet.fromString(pSubnet);
        _staticDns.fromString(pDns);
        _useDHCP = false;

        // if we have a static configuration, use it
        if (!WiFi.config(_staticIP, _staticGateway, _staticSubnet, _staticDns)) {
            Serial.println("[KSHomeKit] Failed to configure static IP address.");
        }
    }
}



void KSHomeKit::tKSHomeKit()
{
	//homeSpan.setLogLevel(1);
	//homeSpan.setLogLevel(2);
  
  homeSpan.setWifiCredentials(wifi_ssid, wifi_password);
  homeSpan.reserveSocketConnections(7);  // need 6 additional connections: NTP - MQTT (2) - WebServer - Telnet - FTP - OTA
  homeSpan.setPortNum(2080);            // default is 80 used for communication between homekit and homespan
  homeSpan.setSketchVersion(_sketchVersion);
 
  // use own default setup code for devices
  //homeSpan.defaultSetupCode = hkDefaultSetupCode;
  homeSpan.setPairingCode(hkDefaultSetupCode);        // we have to switch to function in new version, because prop is now private

  WiFi.persistent(false);
  WiFi.setAutoReconnect(true);


  // set suffix to MAC-Address
  uint8_t macaddr[6];
  WiFi.macAddress(macaddr);

  //int name_len = snprintf(NULL, 0, "-%02X%02X%02X%02X%02X%02X", macaddr[0], macaddr[1], macaddr[2], macaddr[3], macaddr[4], macaddr[5]);
  int name_len = snprintf(NULL, 0, "-%02X%02X%02X", macaddr[3], macaddr[4], macaddr[5]);
  char *name_value = (char*)malloc(name_len + 1);
  //snprintf(name_value, name_len + 1, "-%02X%02X%02X%02X%02X%02X", macaddr[0], macaddr[1], macaddr[2], macaddr[3], macaddr[4], macaddr[5]);
  snprintf(name_value, name_len + 1, "-%02X%02X%02X", macaddr[3], macaddr[4], macaddr[5]);
  homeSpan.setHostNameSuffix(name_value);

  
  // call the specific Homespan implementation
  if (onSetupHomeSpanListener != NULL) {
      onSetupHomeSpanListener();
  }

	//vTaskDelay(pdMS_TO_TICKS(500));
  // TODO: Better wait for Wifi-Connection in hEventGroupNetwork


	// main loop homekit
  for (;;) {
		homeSpan.poll();

		vTaskDelay(pdMS_TO_TICKS(100));
	}
}



#endif     // #if defined USE_KSHomeKit || defined KSLIBRARIES_USEALL