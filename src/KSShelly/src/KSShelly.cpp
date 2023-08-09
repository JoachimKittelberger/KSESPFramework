/**
 * @file KSShelly.h
 * 
 * @brief controll shelly
 * 
 * @details implementation of KSShelly class
 * 
 * @see
 * 
 * @author Joachim Kittelberger <jkittelberger@kibesoft.de>
 * @date 30.04.2022
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

#if defined USE_KSShelly || defined KSLIBRARIES_USEALL     // include File in Build only if it ist defined to use it


#include "KSShelly.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>

#include "KSLogger/src/KSLogger.h"





/*
// JSON um über mqtt den shelly zu schalten bzw. Status auszuwerten
	home/Terrasse/events			// wird beim ein/ausschalten versendet, egal woher geschaltet wird
	rpc = {
		"src":"shellyplus1-441793a5d2fc",
		"dst":"home/Terrasse/events",
		"method":"NotifyStatus",
		"params":{
			"ts":1652704957.52,
			"switch:0":{
				"id":0,
				"output":true,
				"source":"HTTP"
			}
		}
	}
	
	home/Terrasse/status			// wird bei einer status-Änderung versendet. source kann dabei auch MQTT sein
	switch:0 = {
		"id":0,
		"source":"HTTP",
		"output":true,
		"temperature":{
			"tC":54.0,
			"tF":129.3
		}
	}

	Send command:
	home/Terrasse/rpc
	Einschalten mit: home/Terrasse
	rpc = {
		"id":123,				// eigene ID der Hardware
		"src": "user_1",		// Benutzer-ID. 
		"method": "Switch.Set",
		"params": {
			"id": 0,
			"on":true
		}
	}

*/






// setzte den Shelly auf den neuen state und liefert den alten state zurück
// http.begin("http://192.168.1.10/rpc/Switch.Set?id=0&on=true"); //HTTP

bool KSShelly::setState(bool bOn, int id) {

    bLastError = false;
    String serverPath = "http://" + _shellyName;
    //serverPath.concat(shellyName);
    serverPath.concat("/rpc/Switch.Set?id=");
    serverPath.concat(id);
    serverPath.concat("&on=");
    serverPath.concat(bOn ? "true" : "false");
    //LOGGER.println(serverPath);

    String payload = httpGETRequest(serverPath.c_str());

    // convert payload
	StaticJsonDocument<128> doc;
    DeserializationError error = deserializeJson(doc, payload);
    if (error) {
        LOGGER.print("Error: KSShelly::setState(...) deserializeJson() failed: ");
        LOGGER.println(error.c_str());
        bLastError = true;
        return false;
    }

    bool lastState = doc["was_on"].as<bool>() == true;
    return lastState;
}




String KSShelly::httpGETRequest(const char* requestURL) {
    HTTPClient http;
    
    // Your IP address with path or Domain name with URL path 
    //LOGGER.print("[HTTP] begin...\n");
    http.begin(requestURL);
    
    // Send HTTP POST request
    int httpResponseCode = http.GET();
    
    String payload = "{}"; 
    
    if (httpResponseCode > 0) {
        //LOGGER.printf("[HTTP] GET... code: %d\n", httpResponseCode);

        if (httpResponseCode == HTTP_CODE_OK) {
            payload = http.getString();
            //LOGGER.println(payload);
        }
    } else {
        LOGGER.printf("[HTTP] GET... failed, error %d: %s\n", httpResponseCode, http.errorToString(httpResponseCode).c_str());
        bLastError = true;
    }

    // Free resources
    http.end();
    
    return payload;
}

//#endif  // #ifdef USE_KSSHELLY     // include File in Build only if it ist defined to use it

// geht noch über http POST (wie curl) und mqtt

/* HTTP POST
String httpPOSTRequest(const char* serverName) {

      WiFiClient client;
      HTTPClient http;
    
      // Your Domain name with URL path or IP address with path
      http.begin(client, serverName);

      // Specify content-type header
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      // Data to send with HTTP POST
      String httpRequestData = "api_key=aPnTH7Ab3k9G5&sensor=BME280&value1=36.00&value2=60.54&value3=954.14";           
      // Send HTTP POST request
      int httpResponseCode = http.POST(httpRequestData);
      
      // If you need an HTTP request with a content type: application/json, use the following:
      //http.addHeader("Content-Type", "application/json");
      //int httpResponseCode = http.POST("{\"api_key\":\"aPnTH7Ab3k9G5\",\"sensor\":\"BME280\",\"value1\":\"36.00\",\"value2\":\"60.54\",\"value3\":\"954.14\"}");

      // If you need an HTTP request with a content type: text/plain
      //http.addHeader("Content-Type", "text/plain");
      //int httpResponseCode = http.POST("Hello, World!");
     
      LOGGER.print("HTTP Response code: ");
      LOGGER.println(httpResponseCode);
        
      // Free resources
      http.end();


      return 
}

*/

#endif      // #if defined USE_KSShelly || defined KSLIBRARIES_USEALL