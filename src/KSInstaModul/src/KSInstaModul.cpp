/**
 * @file KSInstaModul.cpp
 *
 * @brief Implementation file for KSInstaModul class
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

#if defined USE_KSInstaModul || defined KSLIBRARIES_USEALL     // include File in Build only if it ist defined to use it

#include "KSInstaModul.h"


uint8_t InstaChannels[8][2] = {    // [8][2]
        { InstaChannel1Plus, InstaChannel1Minus },
        { InstaChannel2Plus, InstaChannel2Minus },
        { InstaChannel3Plus, InstaChannel3Minus },
        { InstaChannel4Plus, InstaChannel4Minus },
        { InstaChannel5Plus, InstaChannel5Minus },
        { InstaChannel6Plus, InstaChannel6Minus },
        { InstaChannel7Plus, InstaChannel7Minus },
        { InstaChannel8Plus, InstaChannel8Minus }
    };

uint8_t InstaGroups[3] = { InstaGroupA, InstaGroupB, InstaGroupC };



TaskHandle_t KSInstaModul::createConnection() {

	int coreID = xPortGetCoreID();
	//Serial.print(F("CoreID: "));
	//Serial.println(coreID);
	
	UBaseType_t setupPriority = uxTaskPriorityGet(NULL);
	//Serial.print(F("setup: priority = "));
	//Serial.println(setupPriority);

	xTaskCreatePinnedToCore(
        [](void* context){ static_cast<KSInstaModul*>(context)->tKSInstaModul(); },
		"tKSInstaModul",
		8192,
		this,           // this-pointer as context
		setupPriority,
		&_htKSInstaModul,
		coreID
		);

	return _htKSInstaModul;
}



void KSInstaModul::tKSInstaModul()
{
    // initialisation
	begin();
	
    // main loop tKSInstaModul
    for (;;) {
        // check for Sync and handle handshake
        // handle new message. wait for complete message

        // hier mutex verwenden. Nur Empfangen, falls aktuell nicht gesendet wird
        xSemaphoreTake(_mutexSerial, portMAX_DELAY);
            handleHandshakeRequest();
            handleNewMessage();
        xSemaphoreGive(_mutexSerial);

		vTaskDelay(pdMS_TO_TICKS(5));
	}
}




void KSInstaModul::begin() {
    _recTimeNotComplete = 0;

    // init and empty serial buffer
	// 8 Datenbits, 0 Parität und 1 Stop-Bit
    // Hardware-Pins for Serial2
    //#define RXD2 16
    //#define TXD2 17
    //	Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);

    _HWSerial.begin(9600, SERIAL_8N1);
    emptyRXQueue();             // Empty the Serial Buffer.

    if (_enablePin > -1) {
    	pinMode(_enablePin, OUTPUT);
	    digitalWrite(_enablePin, HIGH);			// enable Funkmodule
    }
}



// leert die Empfangsqueue und liefert die Anzahl Element zurück, die darin waren.
int KSInstaModul::emptyRXQueue() {
    int charCountInQueue = 0;
    while (available() > 0) {
        uint8_t byte = read();
        //Serial.print(' ');
        //Serial.print(to_hex[(byte & 0xF0) >> 4]);
        //Serial.print(to_hex[byte & 0x0F]);
        charCountInQueue++;
    }

    return charCountInQueue;
}



bool KSInstaModul::handleHandshakeRequest() {
    bool bSyncHandled = false;
    
    // if Data is available, read it
    if (available() > 0) {
        // test if we have a handshake signal
        while (available() > 0 && peek() == 0xFA) {
            read();
            write(0x05);     // respond handshake
            flush();
          
            //Serial.println("HandShake ok");
            bSyncHandled = true;
        }
    }
    return bSyncHandled;
}



bool KSInstaModul::handleNewMessage() {
    bool bRightProtocol = false;
    
    if ((available() > 0) && (peek() != 0xFA)) {
        // handle only complete messages
        if (available() < InstaBUFFER_SIZE_TELEGRAMM) {
            _recTimeNotComplete++;
           	vTaskDelay(pdMS_TO_TICKS(1));
        }
        // if we have x times no complete message, empty the serial buffer and try to resync
        
        // Problem bei der Anzahl ist wohl, dass bei jedem Durchlauf nur ein Zeichen eingelesen wird
        // TODO herausfinden, warum dies so ist, sollte doch interrupt-gesteuert sein.
        if (_recTimeNotComplete > 20) {    // 20ms delay should be enough
            
            int bytesInQueue = emptyRXQueue();
            Serial.printf("Error _recTimeNotComplete: %d bytes in RX queue.\n", bytesInQueue);

            _recTimeNotComplete = 0;
        }
        
        
        while (available() >= InstaBUFFER_SIZE_TELEGRAMM) {
            if (peek() == 0x55) {
                // we have a complete message
                
                // read the message into the internal buffer
                uint8_t buf[InstaBUFFER_SIZE_TELEGRAMM];

                buf[0] = read();      // ESC 0x55
                buf[1] = read();      // Steuerwort 1
                buf[2] = read();      // Steuerwort 2
                buf[3] = read();      // Datenbyte 1
                buf[4] = read();      // Datenbyte 2
                buf[5] = read();      // Datenbyte 3
                buf[6] = read();      // Datenbyte 4
                buf[7] = read();      // Datenbyte 5
                buf[8] = read();      // Datenbyte 6
                buf[9] = read();      // Datenbyte 7
                buf[10] = read();     // CRC
                buf[11] = read();     // EOI 0xAA
                
                // check CRC
                uint8_t calcCRC = calculateCRC(buf);
                if (calcCRC != buf[10] || buf[11] != 0xAA) {
                    // inconsistent message
                    Serial.print("Error CRC");
                } else {
                    bRightProtocol = true;
                }
                
                //printTelegram(buf);
                processNewMessage(buf);
            } else {
                // try to sync to next message
                while (available() && peek() != 0x55) {
                    read();
                    Serial.print("SyncError ");
                }
            }
            _recTimeNotComplete = 0;
        }
    }
    return bRightProtocol;
}



void KSInstaModul::processNewMessage(uint8_t* pBuffer) {
    uint8_t group = 0;
    uint8_t channel = 0;
    bool plus = true;
    uint8_t lightScene = 0;
    bool master = false;
    bool allOnOff = false;
    
    bool switchOn = true;

    // store Statusword and Databytes localy
    uint8_t SW1 = pBuffer[1];       // nur wegen Bestückungsabfrage
    uint8_t SW2 = pBuffer[2];       // nur wegen Bestückungsabfrage
    uint8_t DB1 = pBuffer[3];
    uint8_t DB2 = pBuffer[4];       // nur wegen Bestückungsabfrage
    uint8_t DB3 = pBuffer[5];

    // test auf Bestückungsabfrage
    if (SW1 == 0xCD) {
        if ((SW2 == 0xF1) && (DB1 == 0xFA)) {
            Serial.print("Board Option: ");
            if (DB2 == 0xAA)
                Serial.print("RX/TX");
            else if (DB2 == 0xF0)
                Serial.print("RX");
            else if (DB2 == 0x0F)
                Serial.print("TX");
            else
                Serial.print("Unknown");
            Serial.println();
            return;
        }
    }

    // Command from transmitter
    if ((DB1 <= 0x17)
            || ((DB1 >= 0x20) && (DB1 <= 0x57))
            || ((DB1 >= 0x60) && (DB1 <= 0x97))) {
        // its a channel key
        channel = (DB1 & 0x07) + 1;
        group = ((DB1 & 0x18) >> 3) + 1;
        plus = (DB1 & 0xC0) == 0x40;
    } else {
        if ((DB1 & 0x07) <= 4) {    // we have a lightscene
            lightScene = (DB1 & 0x07) + 1;
            if ((DB1 & 0xF0) != 0xD0) {   // we have a Masterkey
                master = true;
                plus = (DB1 & 0xC0) == 0x40;
            }
        } else {       // we have all OnOff
            allOnOff = true;
            plus = (DB1 & 0x01) == 0x01;
        }
    }
    
    // we have a stop message from a longmessage
    if ((DB1 & 0xC0) == 0x00) {
        switchOn = false;
    }
 
    // we have the last message from a shortmessage
    if (((DB1 & 0xC0) != 0x00) && ((DB3 & 0xC0) == 0x40)) {
        switchOn = false;
    }

    // print out the detected commands
    if (channel) {
        Serial.print("C");
        Serial.print(channel);
        if (plus)
            Serial.print("+");
        else
            Serial.print("-");
        if (group == 1)
            Serial.print("A");
        else if (group == 2)
            Serial.print("B");
        else
            Serial.print("C");
    } else if (lightScene || master) {
        if (lightScene)
            Serial.print("S");
        if (master) {
            Serial.print("M");
            if (plus)
                Serial.print("+");
            else
                Serial.print("-");
        }
        Serial.print(lightScene);
    }
    else if (allOnOff) {
        Serial.print("All");
        if (plus)
            Serial.print("+");
        else
            Serial.print("-");
    }
    
    if (switchOn) {
        Serial.print("On ");
    } else {
        Serial.print("Off ");
    }
    Serial.println();

    // if callback is registered, call callback
    if (onNewMessage) {
        onNewMessage(group, channel, plus, switchOn);
    }

    //printTelegram(pBuffer);
}


// DB3 - DB7 are always 0
bool KSInstaModul::sendTelegram(bool useHandshake, uint8_t SW1, uint8_t SW2, uint8_t DB1, uint8_t DB2)
{
    // create a new telegram
    uint8_t buf[InstaBUFFER_SIZE_TELEGRAMM];
    buf[0] = 0x55;      // ESC
    buf[1] = SW1;       // Steuerwort 1
    buf[2] = SW2;       // Steuerwort 2
    buf[3] = DB1;       // Datenbyte 1
    buf[4] = DB2;       // Datenbyte 2
    buf[5] = 0x00;      // Datenbyte 3
    buf[6] = 0x00;      // Datenbyte 4
    buf[7] = 0x00;      // Datenbyte 5
    buf[8] = 0x00;      // Datenbyte 6
    buf[9] = 0x00;      // Datenbyte 7
    buf[10] = calculateCRC(buf);     // CRC
    buf[11] = 0xAA;     // EOI
    
    // hier ein Mutex verwenden, damit nicht gleichzeitig im InstaTask gelesen wird. Evtl. Zeit für Delay verringern
    xSemaphoreTake(_mutexSerial, portMAX_DELAY);

    // Sende Handshake
    if (useHandshake) {
        if (!startHandshake()) {
            Serial.println("Error No Handshake in sendTelegram");
            xSemaphoreGive(_mutexSerial);
            return false;     // Error. No RxTx-Board
        }
    }
    
    // send telegram
    write(buf, sizeof(buf));
    flush();

    xSemaphoreGive(_mutexSerial);

    //printTelegram(buf);
    return true;
}




// Darstellung Byte 4-6 für Funktprotokolle. Gilt nicht für Kommunikation direkt mit Insta-Modul
// Kurztelegramm Betätigung < 0,5s
// 92 12 01
// 92 12 41 (Beim loslassen)

// Zwischen 0,5s und 1s
// 92 12 01
// 92 12 81
// 12 12 81 (Stop beim loslassen)

// bis 12 s Langtelegramm
// 92 12 01
// 92 12 81
// 92 12 C1
// 12 12 C1 (Stop beim loslassen)


bool KSInstaModul::sendShortTelegram(uint8_t key, uint8_t group) {
    uint8_t DB1 = key | group;

    return sendTelegram(true, 0x16, 0x00, DB1, InstaSHORTMESSAGE);
}



// sende eine Longmessage und warte die duration-time und sende dann eine Stopp-Message
bool KSInstaModul::sendLongTelegram(uint8_t key, uint8_t group, int duration) {
    bool bError = false;
    if (duration > 12000) {
        Serial.printf("Warning: Set duration %d to 12000.\n", duration);
        duration = 12000;           // Maximum duration is 12sec
    }
    
    uint8_t DB1 = key | group;
    if (!sendTelegram(true, 0x16, 0x00, DB1, InstaLONGMESSAGE))
        bError = true;

  	vTaskDelay(pdMS_TO_TICKS(duration));
  
    if (!sendTelegram(true, 0x16, 0x00, DB1, InstaSTOPMESSAGE))
        bError = true;

    if (bError) {
        Serial.println("Error sendLongTelegram");
    }

    return !bError;
}



bool KSInstaModul::startHandshake() {
    uint8_t counter = 0;
    uint8_t counterTimes = 0;
    
    while (counterTimes < 5) {    // try it maximum 5 times
        write(0xFA);
        flush();
      	vTaskDelay(pdMS_TO_TICKS(1));
        
        while (counter < 20) {       // wait maximum 20ms
            if (available() > 0) {
                char ch = peek();
                if (ch == 0x05) {
                    read();
                    //Serial.println("startHandshake ok");
                    return true;
                } else {
                    Serial.println("Error: startHandshake");
                    return false;
                }
            } else {
                counter++;
              	vTaskDelay(pdMS_TO_TICKS(1));
            }
        }
        counterTimes++;
    }

    Serial.println("Error: startHandshake");
    return false;
}




uint8_t KSInstaModul::calculateCRC(uint8_t* pBuffer) {
    uint16_t temp = 0;
    for (uint8_t i = 0; i < 10; i++)
        temp = temp + pBuffer[i];
    
    // Zweierkomplement berechnen
    temp = temp & 0xFF;
    temp = 0 - temp;
    
    uint8_t crc = (uint8_t)temp;
    return crc;
}




bool KSInstaModul::getBoardFeatures() {
    // Sende Bestückungsanfrage: Ergebnis wird dann über empfangenes Telegramm in processNewMessage zurückgemeldet
    return sendTelegram(true, 0x32, 0xCD, 0xF1, 0xFA);
}




void KSInstaModul::printTelegram(uint8_t* pBuffer) {
    // print to the LCD
    for (uint8_t i = 0; i < InstaBUFFER_SIZE_TELEGRAMM; i++) {
        switch (i) {
            case 0:
                //continue;
                break;
            case 1:
                //continue;
                break;
            case 2:
                if (pBuffer[i] == 0xEF) {
                    Serial.print("RECV:");
                }
                else {
                    Serial.print("SEND:");
                }
                //continue;
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                break;
            case 6:
                //continue;
                break;
            case 7:
                //continue;
                break;
            case 8:
                //continue;
                break;
            case 9:
                //continue;
                break;
            case 10:
                //continue;
                break;
            case 11:
                //Serial.print(' ');
                //continue;
                break;
                
            default:
                break;
        }
        
        // convert into hex
        Serial.print(to_hex[(pBuffer[i] & 0xF0) >> 4]);
        Serial.print(to_hex[pBuffer[i] & 0x0F]);
        Serial.print(' ');
    }
    Serial.println();
}





/*
    Für Jung mqtt command senden:
	home/insta/events
	rpc = {
		"src": "InstaFunkSender",		// String of source. Am besten Gerätename
        "dst": "home/insta/events",
		"method": "Button.SetShort",
		"params": {
			"group":1,			// Group number 1 - 3
			"channel":8,		// Channel number 1 - 8
			"up":true,			// up or down button
			"on":true,			// on or off
		}
	}

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
	*/

String KSInstaModul::generateRpcString(String src, uint8_t group, uint8_t channel, bool plus, bool switchOn) {
 
    //JSON zusammenbauern und Serialisieren.
	StaticJsonDocument<512> doc;
	doc["src"] = src;
	doc["dst"] = "home/insta/events";
	doc["method"] = "Button.SetShort";

	JsonObject params = doc.createNestedObject("params");
    params["group"] = group;
    params["channel"] = channel;
    params["up"] = plus;
    params["on"] = switchOn;

    String output;
    serializeJson(doc, output);
	//Serial.println(output);

	return output;
}



#endif      // #if defined USE_KSInstaModul || defined KSLIBRARIES_USEALL