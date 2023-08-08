/**
 * @file KSAppInfo.cpp
 * 
 * @brief implementation for KSAppInfo class
 * 
 * @details handles application information
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

#if defined USE_KSAppInfo || defined KSLIBRARIES_USEALL    // include File in Build only if it ist defined to use it

#include "KSAppInfo.h"


String KSAppInfo::listESPStateJSON() {

	const size_t bufferSize = JSON_OBJECT_SIZE(10) + 1500;

	DynamicJsonDocument jsonBuffer(bufferSize);

	JsonObject root = jsonBuffer.to<JsonObject>();

	root["heap"] = ESP.getFreeHeap();
	root["sketch_size"] = ESP.getSketchSize();
	root["free_sketch_space"] = ESP.getFreeSketchSpace();
	root["flash_chip_size"] = ESP.getFlashChipSize();
	root["sdk_version"] = ESP.getSdkVersion();
	root["cpu_freq"] = ESP.getCpuFreqMHz();
	root["chip_model"] = ESP.getChipModel();
	root["chip_cores"] = ESP.getChipCores();
	root["chip_revision"] = ESP.getChipRevision();

	String outputString = "";
	serializeJson(root, outputString);

	return outputString;
}





#ifdef TEST_APPINFO



welcomeMsg = strdup(TELNETSPY_WELCOME_MSG);
	rejectMsg = strdup(TELNETSPY_REJECT_MSG);
void TelnetSpy::setWelcomeMsg(char* msg) {
	if (welcomeMsg) {
		free(welcomeMsg);
	}
	welcomeMsg = strdup(msg);
}


Serial.printf("FireBeetle active\r\n" \
                " Compiled at: " __DATE__ " - " __TIME__ "\r\n" \
                " ESP-IDF: %s\r\n", esp_get_idf_version());



Serial.printf("Sketch size: %u\n", ESP.getSketchSize());
  Serial.printf("Free size: %u\n", ESP.getFreeSketchSpace());
TelnetClient[i].print("Free Heap RAM: ");
        TelnetClient[i].println(ESP.getFreeHeap());

void printSketchInfo(char const* currentProject) {
	Serial.printf("\n\nSketchname: %s\nBuild: %s\t\tIDE: %d.%d.%d\nESP-SDK: %s\n",
					(currentProject), (__TIMESTAMP__), ARDUINO / 10000, ARDUINO % 10000 / 100, ARDUINO % 100 / 10 ? ARDUINO % 100 : ARDUINO % 10, String(ESP.getSdkVersion()).c_str());
}


chipid=ESP.getEfuseMac();//The chip ID is essentially its MAC address(length:6bytes)
Serial.printf("ESP32 Chip ID = %04X",(uint16_t)(chipid>>32));//print High 2 bytes
Serial.printf("%08X\n",(uint32_t)chipid);//print Low 4bytes.




void systeem() {

  uint32_t realSize = ESP.getFlashChipRealSize();
  uint32_t ideSize = ESP.getFlashChipSize();
  String ideSizeS = String(ideSize);

  FlashMode_t ideMode = ESP.getFlashChipMode();
  uint32_t ideID = ESP.getFlashChipId();
  String ideIDs = String(ESP.getFlashChipId(), HEX);
  //add leading zeros
  byte idegap = 8 - ideIDs.length();
  ideIDs.toUpperCase();
  //Serial.println(idegap);
  for (i = 0; i < idegap; i++)
  {
    ideIDs = "0" + ideIDs;
  }
  //https://arduino-esp8266.readthedocs.io/en/latest/libraries.html
  String core = ESP.getCoreVersion();
  client.publish("home/template/stat/core", core.c_str());
  String sdk = String(ESP.getSdkVersion());
  client.publish("home/template/stat/sdk", sdk.c_str());
  String reason = ESP.getResetReason();
  client.publish("home/template/stat/reeason", reason.c_str());
  uint8_t fragmentation = ESP.getHeapFragmentation();
  client.publish("home/template/stat/fragmentation", String(fragmentation).c_str());
  uint32_t cyclecount = ESP.getCycleCount();
  String cyclecountS = String(ESP.getCycleCount(), HEX);
  cyclecountS.toUpperCase();
  client.publish("home/template/stat/cyclecount", String(cyclecount).c_str());
  //client.publish("home/template/stat/cyclecounthex",String(ESP.getCycleCount(),HEX).toUpperCase().c_str());
  client.publish("home/template/stat/cyclecounthex", cyclecountS.c_str());



	
void ICACHE_FLASH_ATTR sendStatus() {
	struct ip_info info;
	FSInfo fsinfo;
	if (!SPIFFS.info(fsinfo)) {
#ifdef DEBUG
		Serial.print(F("[ WARN ] Error getting info on SPIFFS"));
#endif
	}
  DynamicJsonBuffer jsonBuffer;
	JsonObject &root = jsonBuffer.createObject();
	root["command"] = "status";
#ifdef OFFICIALBOARD
	root["board"] = "brdV2";
#endif
	root["heap"] = ESP.getFreeHeap();
	root["chipid"] = String(ESP.getChipId(), HEX);
	root["cpu"] = ESP.getCpuFreqMHz();
	root["sketchsize"] = ESP.getSketchSize();
	root["availsize"] = ESP.getFreeSketchSpace();
	root["availspiffs"] = fsinfo.totalBytes - fsinfo.usedBytes;
	root["spiffssize"] = fsinfo.totalBytes;
	root["uptime"] = NTP.getDeviceUptimeString();
	root["version"] = VERSION;

	if (inAPMode) {
		wifi_get_ip_info(SOFTAP_IF, &info);
		struct softap_config conf;
		wifi_softap_get_config(&conf);
		root["ssid"] = String(reinterpret_cast<char *>(conf.ssid));
		root["dns"] = printIP(WiFi.softAPIP());
		root["mac"] = WiFi.softAPmacAddress();
	} else {
		wifi_get_ip_info(STATION_IF, &info);
		struct station_config conf;
		wifi_station_get_config(&conf);
		root["ssid"] = String(reinterpret_cast<char *>(conf.ssid));
		root["dns"] = printIP(WiFi.dnsIP());
		root["mac"] = WiFi.macAddress();
	}

	IPAddress ipaddr = IPAddress(info.ip.addr);
	IPAddress gwaddr = IPAddress(info.gw.addr);
	IPAddress nmaddr = IPAddress(info.netmask.addr);
	root["ip"] = printIP(ipaddr);
	root["gateway"] = printIP(gwaddr);
	root["netmask"] = printIP(nmaddr);

	size_t len = root.measureLength();
	AsyncWebSocketMessageBuffer *buffer = ws.makeBuffer(len);
	if (buffer) {
		root.printTo((char *)buffer->get(), len + 1);
		ws.textAll(buffer);
	}
  }



  //Serial.printf("Flash real id:   %08X\n", ESP.getFlashChipId());
  //Serial.printf("Flash real size: %u bytes\n\n", realSize);

  //Serial.printf("Flash ide  size: %u bytes\n", ideSize);
  //Serial.printf("Flash ide speed: %u Hz\n", ESP.getFlashChipSpeed());
  //Serial.printf("Flash ide mode:  %s\n", (ideMode == FM_QIO ? "QIO" : ideMode == FM_QOUT ? "QOUT" : ideMode == FM_DIO ? "DIO" : ideMode == FM_DOUT ? "DOUT" : "UNKNOWN"));
  String idemode = ("Flash ide mode:  %s\n", (ideMode == FM_QIO ? "QIO" : ideMode == FM_QOUT ? "QOUT" : ideMode == FM_DIO ? "DIO" : ideMode == FM_DOUT ? "DOUT" : "UNKNOWN"));
  client.publish("home/template/stat/idemode", idemode.c_str());
  if (ideSize != realSize) {
    //Serial.println("Flash Chip configuration wrong!\n");
  } else {
    //Serial.println("Flash Chip configuration ok.\n");
  }


  client.publish("home/template/stat/ChipID", ideIDs.c_str());
  client.publish("home/template/stat/memory", ideSizeS.c_str());
  client.publish("home/template/stat/realmem", String(realSize).c_str());
}




void ICACHE_FLASH_ATTR printScanResult(int networksFound) {
	// sort by RSSI
	int n = networksFound;
	int indices[n];
	int skip[n];
	for (int i = 0; i < networksFound; i++) {
		indices[i] = i;
	}
	for (int i = 0; i < networksFound; i++) {
		for (int j = i + 1; j < networksFound; j++) {
			if (WiFi.RSSI(indices[j]) > WiFi.RSSI(indices[i])) {
				std::swap(indices[i], indices[j]);
				std::swap(skip[i], skip[j]);
			}
		}
	}
	DynamicJsonBuffer jsonBuffer;
	JsonObject &root = jsonBuffer.createObject();
	root["command"] = "ssidlist";
	JsonArray &scan = root.createNestedArray("list");
	for (int i = 0; i < 5 && i < networksFound; ++i) {
		JsonObject &item = scan.createNestedObject();
		item["ssid"] = WiFi.SSID(indices[i]);
		item["bssid"] = WiFi.BSSIDstr(indices[i]);
		item["rssi"] = WiFi.RSSI(indices[i]);
		item["channel"] = WiFi.channel(indices[i]);
		item["enctype"] = WiFi.encryptionType(indices[i]);
		item["hidden"] = WiFi.isHidden(indices[i]) ? true : false;
	}
	size_t len = root.measureLength();
	AsyncWebSocketMessageBuffer *buffer = ws.makeBuffer(len); //  creates a buffer (len + 1) for you.
	if (buffer) {
		root.printTo((char *)buffer->get(), len + 1);
		ws.textAll(buffer);
	}
	WiFi.scanDelete();
}



void ICACHE_FLASH_ATTR sendUserList(int page, AsyncWebSocketClient *client) {
	DynamicJsonBuffer jsonBuffer;
	JsonObject &root = jsonBuffer.createObject();
	root["command"] = "userlist";
	root["page"] = page;
	JsonArray &users = root.createNestedArray("list");
	Dir dir = SPIFFS.openDir("/P/");
	int first = (page - 1) * 15;
	int last = page * 15;
	int i = 0;
	while (dir.next()) {
		if (i >= first && i < last) {
			JsonObject &item = users.createNestedObject();
			String uid = dir.fileName();
			uid.remove(0, 3);
			item["uid"] = uid;
			File f = SPIFFS.open(dir.fileName(), "r");
			size_t size = f.size();
			std::unique_ptr<char[]> buf(new char[size]);
			f.readBytes(buf.get(), size);
			DynamicJsonBuffer jsonBuffer2;
			JsonObject &json = jsonBuffer2.parseObject(buf.get());
			if (json.success()) {
				String username = json["user"];
				int AccType = json["acctype"];
				unsigned long validuntil = json["validuntil"];
				item["username"] = username;
				item["acctype"] = AccType;
				item["validuntil"] = validuntil;
			}
		}
		i++;
	}
	float pages = i / 15.0;
	root["haspages"] = ceil(pages);
	size_t len = root.measureLength();
	AsyncWebSocketMessageBuffer *buffer = ws.makeBuffer(len);
	if (buffer) {
		root.printTo((char *)buffer->get(), len + 1);
		if (client) {
			client->text(buffer);
			client->text("{\"command\":\"result\",\"resultof\":\"userlist\",\"result\": true}");
		} else {
			ws.textAll("{\"command\":\"result\",\"resultof\":\"userlist\",\"result\": false}");
		}
	}
}


void ICACHE_FLASH_ATTR sendTime() {
	DynamicJsonBuffer jsonBuffer;
	JsonObject &root = jsonBuffer.createObject();
	root["command"] = "gettime";
	root["epoch"] = now();
	root["timezone"] = timeZone;
	size_t len = root.measureLength();
	AsyncWebSocketMessageBuffer *buffer = ws.makeBuffer(len);
	if (buffer) {
		root.printTo((char *)buffer->get(), len + 1);
		ws.textAll(buffer);
	}
}


#endif




#endif		// #if defined USE_KSAppInfo || defined KSLIBRARIES_USEALL