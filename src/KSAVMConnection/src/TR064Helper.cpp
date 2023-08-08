/**
 * @file TR064Helper.cpp
 * 
 * @brief Implementation for TR064 Helper Functions
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


#include "TR064Helper.h"


extern TR064 fbConnection;


/////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// helper methods//////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

/** 
 *  Get the number of devices that were connected to the WIFI lastly
 *  (some of them might not be online anymore, you need to check them individually!)
 *  return (int)
 */
int getWifiNumber() {
	String params[][2] = {{}};
	String req[][2] = {{"NewTotalAssociations", ""}};
	fbConnection.action("urn:dslforum-org:service:WLANConfiguration:1", "GetTotalAssociations", params, 0, req, 1);
	int numDev = (req[0][1]).toInt();
	return numDev;
}

/** Print the status of all devices that were connected to the WIFI lastly
*  (some of them might not be online anymore, also gets you the hostnames and macs)
*  return nothing as of yet
 */
void getStatusOfAllWifi() {
	getStatusOfAllWifi(getWifiNumber());
}


/** 
 *  Print the status of all devices that were connected to the WIFI lastly
 * (some of them might not be online anymore, also gets you the hostnames and macs)
 * return nothing as of yet
 */
void getStatusOfAllWifi(int numDev) {
	//Query the mac and status of each device
	for (int i=0;i<numDev;++i) {
		String params[][2] = {{"NewAssociatedDeviceIndex", String(i)}};
		String req[][2] = {{"NewAssociatedDeviceAuthState", ""}, {"NewAssociatedDeviceMACAddress", ""}, {"NewAssociatedDeviceIPAddress", ""}};
		fbConnection.action("urn:dslforum-org:service:WLANConfiguration:1", "GetGenericAssociatedDeviceInfo", params, 1, req, 2);
		if(Serial) {
			Serial.printf("%d:\t", i);
			Serial.println((req[1][1])+" is online "+(req[0][1]));
			Serial.flush();
		}
	}
}

/** 
 *  Get the status of one very specific device. Only works if it is a WIFI device!
 * return nothing, but fills the array r
 */
void getStatusOfMACwifi(String mac, String (&r)[4][2]) {
	// Ask for one specific device
	mac.toUpperCase();
	String params[][2] = {{"NewAssociatedDeviceMACAddress", mac}};
	String req[][2] = {{"NewAssociatedDeviceIPAddress", ""}, {"NewAssociatedDeviceAuthState", ""}};
	fbConnection.action("urn:dslforum-org:service:WLANConfiguration:1", "GetSpecificAssociatedDeviceInfo", params, 1, req, 2);
	if(Serial) {
		Serial.println(mac + " is online " + (req[2][1]));
		Serial.flush();
	}
	r[STATUS_MAC_INDEX][0] = STATUS_MAC;
	r[STATUS_MAC_INDEX][1] = mac;
	r[STATUS_IP_INDEX][0] = STATUS_IP;
	r[STATUS_IP_INDEX][1] = req[0][1];
	r[STATUS_HOSTNAME_INDEX][0] = STATUS_HOSTNAME;
	r[STATUS_HOSTNAME_INDEX][1] = "";
	r[STATUS_ACTIVE_INDEX][0] = STATUS_ACTIVE;
	r[STATUS_ACTIVE_INDEX][1] = req[1][1];
}

/** 
 *  Get the number of devices that were connected to the router (ever)
 *  (some of them might not be online, you need to check them individually!)
 *  return (int)
 */
int getDeviceNumber() {
	String params[][2] = {{}};
	String req[][2] = {{"NewHostNumberOfEntries", ""}};
	fbConnection.action("urn:dslforum-org:service:Hosts:1", "GetHostNumberOfEntries", params, 0, req, 1);
	int numDev = (req[0][1]).toInt();
	return numDev;
}

/** 
 *  Get the status of one very specific device. May contain less information as the same option for WIFI.
 * return nothing, but fills the array r
 */
void getStatusOfMAC(String mac, String (&r)[4][2]) {
	//Ask for one specific device
	String params[][2] = {{"NewMACAddress", mac}};
	String req[][2] = {{"NewIPAddress", ""}, {"NewActive", ""}, {"NewHostName", ""}};
	fbConnection.action("urn:dslforum-org:service:Hosts:1", "GetSpecificHostEntry", params, 1, req, 2);
	if(Serial) {
		Serial.println(mac + " is online " + (req[1][1]));
		Serial.flush();
	}
	r[STATUS_MAC_INDEX][0] = STATUS_MAC;
	r[STATUS_MAC_INDEX][1] = mac;
	r[STATUS_IP_INDEX][0] = STATUS_IP;
	r[STATUS_IP_INDEX][1] = req[0][1];
	r[STATUS_HOSTNAME_INDEX][0] = STATUS_HOSTNAME;
	r[STATUS_HOSTNAME_INDEX][1] = req[2][1];
	r[STATUS_ACTIVE_INDEX][0] = STATUS_ACTIVE;
	r[STATUS_ACTIVE_INDEX][1] = req[1][1];
}


/**
 * Prints the status of the device on the screen (arrays r of the getStatusOfMAC methods).
 * return nothing
 */
void verboseStatus(String r[4][2]) {
	for (int i=0;i<4;++i) {
		if(Serial) Serial.print(r[i][0]+"="+r[i][1]+", ");
	}
	if(Serial) Serial.print("\n");
}

#endif		// #if defined USE_KSAVMConnection || defined KSLIBRARIES_USEALL