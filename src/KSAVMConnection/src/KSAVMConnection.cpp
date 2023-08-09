/**
 * @file KSAVMConnection.cpp
 * 
 * @brief implementation for KSAVMConnection class
 * 
 * @details handles communication to fritz.box homeautomation-switches
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

#if defined USE_KSAVMConnection || defined KSLIBRARIES_USEALL    // include File in Build only if it ist defined to use it


#include "KSAVMConnection.h"

#include "KSLogger/src/KSLogger.h"


const char *SwStates[] =
{
    "OFF",
    "ON",
    "TOGGLE",
    "UNDEFINED"
};

const char *PresentStates[] =
{
    "DISCONNECTED",
    "REGISTERED",
    "CONNECTED",
    "UNKNOWN"
};




KSAVMConnection::KSAVMConnection() : _fbConnection(fbPort, fbIp, fbUsername, fbPassword) {

}


void KSAVMConnection::init() {
	_fbConnection.init();
}






void KSAVMConnection::ensureWIFIConnection() {
  if (WiFi.status() != WL_CONNECTED) {
    LOGGER.print("Connecting to ");
    LOGGER.println(WiFi.SSID());
    while (WiFi.status() != WL_CONNECTED) {
      vTaskDelay(pdMS_TO_TICKS(500));
      LOGGER.print(".");
    }
    LOGGER.print("IP number is ");
    LOGGER.println(WiFi.localIP());
  }
}



bool KSAVMConnection::isWIFIConnected() {
  if (WiFi.status() != WL_CONNECTED) {
    return false;
  }
  return true;
}



void KSAVMConnection::SetSwitchState(String AIN, SwState state) {
    if (!isWIFIConnected()) {
      LOGGER.println("No WIFI-Connection in KSAVMConnection::SetSwitchState()");
      return;
    }
    //ensureWIFIConnection();

    String params[][2] = {{"NewAIN", AIN}, {"NewSwitchState", SwStates[state]}};
    _fbConnection.action("urn:dslforum-org:service:X_AVM-DE_Homeauto:1", "SetSwitch", params, 2);

    //LOGGER.println("SetSwitchState()");

    //LOGGER.print("Set Switch state of ");
    //LOGGER.print(AIN);
    //LOGGER.print(" to: ");
    //LOGGER.println(state);
}



bool KSAVMConnection::GetSwitchState(String AIN) {
  if (!isWIFIConnected()) {
      LOGGER.println("No WIFI-Connection in KSAVMConnection::GetSwitchState()");
      return false;
  }
  //ensureWIFIConnection();

  String params[][2] = {{"NewAIN", AIN}};
  String req[][2] = {{"NewDeviceName", ""}, {"NewSwitchState", ""}, {"NewPresent", ""}};
  _fbConnection.action("urn:dslforum-org:service:X_AVM-DE_Homeauto:1", "GetSpecificDeviceInfos", params, 1, req, 3);
  String name = req[0][1];
  String state = req[1][1];
  String present = req[2][1];

    //LOGGER.println("GetSwitchState()");
/*
    LOGGER.print("Get Switch state of ");
    LOGGER.print(name);
    LOGGER.print(": ");
    LOGGER.print(state);
    LOGGER.print(" Presence: ");
    LOGGER.println(present);
*/
  bool bRetVal = false;
  if (state == SwStates[SwState::ON]) {
    bRetVal = true;
  } else if (state == SwStates[SwState::OFF]) {
    bRetVal = false;
  }
  return bRetVal;
}


bool KSAVMConnection::IsSwitchPresent(String AIN) {
    if (!isWIFIConnected()) {
       LOGGER.println("No WIFI-Connection in KSAVMConnection::IsSwitchPresent()");
      return false;
    }
    //ensureWIFIConnection();

    String params[][2] = {{"NewAIN", AIN}};
    String req[][2] = {{"NewPresent", ""}};
    _fbConnection.action("urn:dslforum-org:service:X_AVM-DE_Homeauto:1", "GetSpecificDeviceInfos", params, 1, req, 1);
    String present = req[0][1];

    if (present == PresentStates[PresentState::CONNECTED]) {
      return true;
    }
    
    LOGGER.print("Switch ");
    LOGGER.print(AIN);
    LOGGER.print(" not present. State: ");
    LOGGER.println(present);

    return false;
}


float KSAVMConnection::GetSwitchTemperature(String AIN) {
    if (!isWIFIConnected()) return -10.0;
    //ensureWIFIConnection();

    String params[][2] = {{"NewAIN", AIN}};
    String req[][2] = {{"NewMultimeterPower", ""}, {"NewTemperatureCelsius", ""}};
    _fbConnection.action("urn:dslforum-org:service:X_AVM-DE_Homeauto:1", "GetSpecificDeviceInfos", params, 1, req, 2);
    float power = req[0][1].toInt() / 100.0;
    float temp = req[1][1].toInt() / 10.0;
/* 
    LOGGER.print("Stromverbrauch: ");
    LOGGER.print(power, 1);
    LOGGER.println("W");
    LOGGER.print("Temperatur: ");
    LOGGER.print(temp, 1);
    LOGGER.println("*C");
*/
    return temp;
}
 


void KSAVMConnection::GetDeviceInfo(String AIN) {
    if (!isWIFIConnected()) return;
    //ensureWIFIConnection();

    String params[][2] = {{"NewAIN", AIN}};
    String req[][2] = {{"NewDeviceName", ""}, {"NewMultimeterPower", ""}, {"NewTemperatureCelsius", ""}};
    _fbConnection.action("urn:dslforum-org:service:X_AVM-DE_Homeauto:1", "GetSpecificDeviceInfos", params, 1, req, 3);
    String name = req[0][1];
    float power = req[1][1].toInt() / 100.0;
    float temp = req[2][1].toInt() / 10.0;
    LOGGER.print(name);
    LOGGER.print(" Stromverbrauch: ");
    LOGGER.print(power, 1);
    LOGGER.println("W");
    LOGGER.print("Temperatur: ");
    LOGGER.print(temp, 1);
    LOGGER.println("*C");
}
 

#endif    // #if defined USE_KSAVMConnection || defined KSLIBRARIES_USEALL