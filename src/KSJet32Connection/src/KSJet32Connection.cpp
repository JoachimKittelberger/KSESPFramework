/**
 * @file KSJet32Connection.cpp
 *
 * @brief Implementation file for KSJet32Connection class
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

#if defined USE_KSJet32Connection || defined KSLIBRARIES_USEALL     // include File in Build only if it ist defined to use it

#include "KSJet32Connection.h"
#include "KSEventGroupNetwork/src/KSEventGroupNetwork.h"
#include "KSLogger/src/KSLogger.h"


TaskHandle_t KSJet32Connection::createConnection(EventGroupHandle_t *phEventGroupNetwork) {

	_phEventGroupNetwork = phEventGroupNetwork;
    resetConnectedEvent();
    resetConnectingEvent();

	int coreID = xPortGetCoreID();
	//LOGGER.print(F("CoreID: "));
	//LOGGER.println(coreID);
	
	UBaseType_t setupPriority = uxTaskPriorityGet(NULL);
	//LOGGER.print(F("setup: priority = "));
	//LOGGER.println(setupPriority);

	xTaskCreatePinnedToCore(
        [](void* context){ static_cast<KSJet32Connection*>(context)->tKSJet32Connection(); },
		"tKSJet32Connection",
		8192,
		this,           // this-pointer as context
		setupPriority,
		&_htKSJet32Connection,
		coreID
		);

	return _htKSJet32Connection;
}



void KSJet32Connection::tKSJet32Connection()
{
    // Warte auf Wifi-Verbindung und Initialisierung:
    // Wenn Connection-Bit noch nicht gesetzt, dann das erste Mal warten.
    if (_phEventGroupNetwork && (*_phEventGroupNetwork != NULL)) {
        if ((xEventGroupGetBits(*_phEventGroupNetwork) & EG_NETWORK_CONNECTED) == 0) {
            //LOGGER.println(F("[ftp] Wating for Event EG_NETWORK_CONNECTED"));
            EventBits_t eventGroupValue;
            eventGroupValue = xEventGroupWaitBits(*_phEventGroupNetwork, (EG_NETWORK_INITIALIZED | EG_NETWORK_CONNECTED), pdFALSE, pdTRUE, portMAX_DELAY);
            //LOGGER.println(F("[ftp] Event EG_NETWORK_CONNECTED set"));
        }
    }

	// main loop Jet32
    for (;;) {
        if (isConnected()) {
            // process incoming packet
            int packetSize = _udpReceive.parsePacket();
            if (packetSize >= 20) {

                uint8_t buffer[50];
                memset(buffer, 0, 50);

                int len = _udpReceive.read(buffer, 50);
                //LOGGER.printf("Received Packet from %s RemotePort: % d with %u bytes", _udpReceive.remoteIP().toString().c_str(), _udpReceive.remotePort(), packetSize);
                //LOGGER.printf(" and length read: %u bytes\n", len);
                processPacketHandler(buffer, len);
            }
        } else {
            connect();
            if (!isConnected()) {
                //disconnect();
    		    vTaskDelay(pdMS_TO_TICKS(2000));
            }
        }

		vTaskDelay(pdMS_TO_TICKS(10));
	}
}



bool KSJet32Connection::isConnected() {
    if (!_bConnected)
        return false;

    // prüfe Wifi Connection-Bit noch nicht gesetzt, dann das erste Mal warten.
    if (_phEventGroupNetwork && (*_phEventGroupNetwork != NULL)) {
        if ((xEventGroupGetBits(*_phEventGroupNetwork) & EG_NETWORK_CONNECTED) != EG_NETWORK_CONNECTED) {
            _bConnected = false;
            resetConnectingEvent();
            resetConnectedEvent();
        }
    }
    return _bConnected;
}



bool KSJet32Connection::connect() {
    disconnect();           // TODO. Evtl wird dies nicht benötigt

    // Warte auf Wifi-Verbindung und Initialisierung:
    // Wenn Connection-Bit noch nicht gesetzt, dann das erste Mal warten.
    if (_phEventGroupNetwork && (*_phEventGroupNetwork != NULL)) {
        if ((xEventGroupGetBits(*_phEventGroupNetwork) & EG_NETWORK_CONNECTED) == 0) {
            //LOGGER.println(F("[Jet32] Wating for Event EG_NETWORK_CONNECTED"));
            EventBits_t eventGroupValue;
            eventGroupValue = xEventGroupWaitBits(*_phEventGroupNetwork, (EG_NETWORK_INITIALIZED | EG_NETWORK_CONNECTED), pdFALSE, pdTRUE, portMAX_DELAY);
            //LOGGER.println(F("[Jet32] Event EG_NETWORK_CONNECTED set"));
        }
    }
    setConnectingEvent();


    // init udp-port
    if (!_udpReceive.begin(_recPort)) {
        LOGGER.printf("connect failed _udpReceive.begin\n");
        return false;
    }
    // get connection state from PLC
    if (getConnectionState(0x11)) { 
        resetConnectingEvent();
        setConnectedEvent();
        _bConnected = true;
    }
    return _bConnected;
}



void KSJet32Connection::disconnect() {
    _udpReceive.stop();
    _udpSend.stop();
    _bNewValueArrived = false;
    _lastValue = 0;
    _bConnected = false;
    resetConnectedEvent();
    resetConnectingEvent();
}



void KSJet32Connection::resetConnectedEvent() {
    // Reset Connected Event
    if (_phEventGroupNetwork && (*_phEventGroupNetwork != NULL)) {
        if ((xEventGroupGetBits(*_phEventGroupNetwork) & EG_NETWORK_JET32_CONNECTED) == EG_NETWORK_JET32_CONNECTED) {
            xEventGroupClearBits(*_phEventGroupNetwork, EG_NETWORK_JET32_CONNECTED);
        }
    }
}


void KSJet32Connection::resetConnectingEvent() {
    // Reset Connecting Event
    if (_phEventGroupNetwork && (*_phEventGroupNetwork != NULL)) {
        if ((xEventGroupGetBits(*_phEventGroupNetwork) & EG_NETWORK_JET32_CONNECTING) == EG_NETWORK_JET32_CONNECTING) {
            xEventGroupClearBits(*_phEventGroupNetwork, EG_NETWORK_JET32_CONNECTING);
        }
    }
}


void KSJet32Connection::setConnectedEvent() {
    // Set Connected Event
    if (_phEventGroupNetwork && (*_phEventGroupNetwork != NULL)) {
        if ((xEventGroupGetBits(*_phEventGroupNetwork) & EG_NETWORK_JET32_CONNECTED) == 0) {
            xEventGroupSetBits(*_phEventGroupNetwork, EG_NETWORK_JET32_CONNECTED);
        }
    }
}



void KSJet32Connection::setConnectingEvent() {
    // Set Connecting Event
    if (_phEventGroupNetwork && (*_phEventGroupNetwork != NULL)) {
        if ((xEventGroupGetBits(*_phEventGroupNetwork) & EG_NETWORK_JET32_CONNECTING) == 0) {
            xEventGroupSetBits(*_phEventGroupNetwork, EG_NETWORK_JET32_CONNECTING);
        }
    }
}



// TODO: -> Kann auch in Jet32DataTelegramm wandern und dort interpretiert werden.
void KSJet32Connection::processPacketHandler(uint8_t* packet, int length) {
    //LOGGER.print("Received ");
    //LOGGER.print(": ");
    //Jet32DataTelegram::printArray(packet, length);
    //LOGGER.println();


    // check if we have a PCOM-Protocol
    if (length >= 20) {        // check minimum data length required
        uint8_t* pData = packet;
        if ((pData[0] == 0x4a) && (pData[1] == 0x57) && (pData[2] == 0x49) && (pData[3] == 0x50)) {     // check Header "JWIP"

            uint32_t comRef = (pData[8] * 256*256*256) + (pData[9] * 256*256) + (pData[10] * 256) + pData[11];

            // TODO: anhand comRef die eigentliche Referenz herausfinden.
            // Anfragen in einer Queue eintragen und mit einer eigenen ComRef versehen. Bei zurücklesen dann auf Anfrage mappen
            
            if (length == 21 ) {       // for writeVariable
                if (pData[20] == 0x20) {          // return PCOM-WriteRegister ok

                    //LOGGER.printf("Write.IntegerRegister comRef: %u ok\n", comRef);
                    _bNewValueArrived = true;
                }


                if (pData[20] == 0xEE) {          // return command 'U' ok
                    LOGGER.printf("Return Command U comRef: %u ok\n", comRef);
                    _lastValue = pData[20];
                }
            }


            // wenn IntRegister:
            else if (length >= 26 ) {       // for readVariable
                if (pData[20] == 0x20) {          // return PCOM-ReadRegister
                    uint8_t dataType = pData[21];   // read type of returnvalue



                    uint32_t value = (pData[22] * 256*256*256) + (pData[23] * 256*256) + (pData[24] * 256) + pData[25];
                    //LOGGER.printf("Read.IntegerRegister comRef: %u: %u\n", comRef, value);
                    _lastValue = value;
                    _bNewValueArrived = true;
                }
            } else {
                LOGGER.println("Wrong Datalenght for Read.IntegerRegister");
            }
        } else {
            LOGGER.println("Other UDP-Packet received. No PCOM");
        }
    } else {
        LOGGER.println("Other UDP-Packet received. No PCOM");
    }




/*
        // Check Header
        if (data.count >= 20) {         // check minimum data length required
            if data[0] == 0x4A && data[1] == 0x57 && data[2] == 0x49 && data[3] == 0x50 {

                // read communication-Reference
                var comRef = (UInt(data[8]) * 256*256*256) + (UInt(data[9]) * 256*256) + (UInt(data[10]) * 256) + UInt(data[11])
                var inValue: UInt = 0
                
                
                // TODO: anhand ComRef die eigentliche Referenz herausfinden und den WErt zurückgeben
                if comRef != 0 {

                    let telegramID = UInt32(comRef)
                    
                    if let offset = PlcDataAccessQueue.firstIndex(where: { $0.telegramID == telegramID }) {
                        
                        let originalComRef = UInt(PlcDataAccessQueue[offset].comRef)
                        
                        comRef = originalComRef
                        
                        let type = PlcDataAccessQueue[offset].type
                        let cmd = PlcDataAccessQueue[offset].cmd
                        let number = PlcDataAccessQueue[offset].number
                        
                        switch type {
                        case .IntegerRegister:
                            
                            if data.count >= 26 {       // for readVariable
                                if data[20] == 0x20 {       // return PCOM-ReadRegister
                                    let datatype = data[21]     // read type of returnvalue
                                    
                                    inValue = (UInt(data[22]) * 256*256*256) + (UInt(data[23]) * 256*256) + (UInt(data[24]) * 256) + UInt(data[25])
                                }
                                
                                // call individual Handler defined in Protocol
                                delegate?.didRedeiveReadIntRegister(UInt(number), with: Int(inValue), tag: comRef)
                            } else {
                                print("wrong Datalength for Read.IntegerRegister")
                            }
                            
                        
                        case .Flag:
                            
                            if data.count >= 21 {
                                // status oder Merker, Ausgangsrückmeldung
                                if data[20] == 0x20 {       // Flag is 0
//                                    print("didReceive ReadFlag reset \(data[20]) with tag: \(comRef)")
                                        
                                    // call individual Handler defined in Protocol
                                    delegate?.didRedeiveReadFlag(UInt(number), with: false, tag: comRef)
                                }
                                else if data[20] == 0x21 {  // Flag is 1
//                                    print("didReceive ReadFlag set \(data[20]) with tag: \(comRef)")
                                        
                                    // call individual Handler defined in Protocol
                                    delegate?.didRedeiveReadFlag(UInt(number), with: true, tag: comRef)
                                }
                                else {
                                     print("didReceive ReadFlag Status \(data[20]) with tag: \(comRef)")
                                }
                            } else {
                                print("wrong Datalength for Read.Flag")
                            }
                        
                            
                        default:
                            print("Datatype not supported!")
                        }
                        PlcDataAccessQueue.remove(at: offset)
                    }
                }
                else {
                    print("didReceive Status \(data[20]) with tag: \(comRef)")
                }
                return

            } else {
                print("didRecieve other protocol from Socket: \(data.hexEncodedString())")
            }
            
        } else {
            print("didRecieve other protocol from Socket: \(data.hexEncodedString())")
        }
    */    

}



bool KSJet32Connection::waitForResponse(int timeoutInMS) {
    bool bRetVal = false;
    long startMillis = millis();
   
    while (millis() - startMillis < timeoutInMS) {
        // check if new Value ist there
        if (_bNewValueArrived) {
            _bNewValueArrived = false;
            bRetVal = true;
            break;          // exit While-Loop
        }
        
        vTaskDelay(pdMS_TO_TICKS(10));
    }  
    _bNewValueArrived = false;
    _bConnected = bRetVal;

    return bRetVal;
}




bool KSJet32Connection::sendTelegramSynchron(Jet32DataTelegram* pMsg, uint32_t comRef, bool bCheckConnected) {
	//ASSERT(pMsg);
	if (!pMsg) return false;
    if (bCheckConnected && !isConnected()) return false;

	bool bRetVal = true;

    csJet32Access.EnterCriticalSection();

    _lastValue = -1;
    WiFiUDP udpTX;
    if (udpTX.beginPacket(IPAddress(_remoteIP), _sendPort) == 1) {
        udpTX.write(pMsg->RawDataPtr(), pMsg->Length());
        udpTX.endPacket();
		
		if (!waitForResponse(_timeoutInMS)) {
	        LOGGER.printf("Timeout waiting for UDP-Response from %s\n", _remoteIP.toString().c_str());
			bRetVal = false;
		}
    } else {
        LOGGER.printf("Cannot connect to UDP: %s:%u\n", _remoteIP.toString().c_str(), _sendPort);
        _bConnected = false;
        bRetVal = false;
    }
    csJet32Access.LeaveCriticalSection();

    return bRetVal;
}



bool KSJet32Connection::sendTelegramAsynchron(Jet32DataTelegram* pMsg, uint32_t comRef, bool bCheckConnected) {
	//ASSERT(pMsg);
	if (!pMsg) return false;
    if (bCheckConnected && !isConnected()) return false;

	bool bRetVal = true;

    csJet32Access.EnterCriticalSection();

    _lastValue = -1;
    WiFiUDP udpTX;
    if (udpTX.beginPacket(IPAddress(_remoteIP), _sendPort) == 1) {
        udpTX.write(pMsg->RawDataPtr(), pMsg->Length());
        udpTX.endPacket();
		
//		if (!waitForResponse(_timeoutInMS)) {
//	        LOGGER.printf("Timeout waiting for UDP-Response from %s\n", _remoteIP.toString().c_str());
//			bRetVal = false;
//		}
    } else {
        LOGGER.printf("Cannot connect to UDP: %s:%u\n", _remoteIP.toString().c_str(), _sendPort);
        _bConnected = false;
        bRetVal = false;
    }
    csJet32Access.LeaveCriticalSection();
    
    return bRetVal;
}




bool KSJet32Connection::getConnectionState(int8_t value) { 
    // send commcand 'U'
    Jet32DataTelegram msg(_recPort);
   	uint32_t comRef = 0x01020304;

    msg.init(Jet32Command::connect, 0, value, comRef);
	if (!sendTelegramAsynchron(&msg, comRef, false)) {      // don't check connection for init connection
        //LOGGER.printf("getConnectionState() failed\n");
		return false;
	}

    csJet32Access.EnterCriticalSection();
    // wait for response
    bool bRetVal = false;
    long startMillis = millis();
    while (millis() - startMillis < _timeoutInMS) {

        // process incoming packet
        int packetSize = _udpReceive.parsePacket();
        if (packetSize >= 20) {

            uint8_t buffer[50];
            memset(buffer, 0, 50);

            int len = _udpReceive.read(buffer, 50);
            //LOGGER.printf("Received Packet from %s RemotePort: % d with %u bytes", _udpReceive.remoteIP().toString().c_str(), _udpReceive.remotePort(), packetSize);
            //LOGGER.printf(" and length read: %u bytes\n", len);
            processPacketHandler(buffer, len);
            if ((uint8_t)_lastValue == (value ^ 0xFF)) {
                bRetVal = true;
                break;
            }
        }
        
        vTaskDelay(pdMS_TO_TICKS(10));
   }  
    csJet32Access.LeaveCriticalSection();

    return bRetVal;
}




// TODO: Define value for Error z.B: MAX_INT
int32_t KSJet32Connection::readIntRegister(uint32_t number) {
    uint32_t comRef = 0x01020304;
    Jet32DataTelegram msg(_recPort);
//        msg.init(Jet32Command::readIntRegister, registerNumber, 0, comRef);
    msg.init(Jet32Command::readVariable, number, 0, comRef);

	if (!sendTelegramSynchron(&msg, comRef)) {
        LOGGER.printf("Read.IntegerReg %u failed\n", number);
		return -1;		// TODO: use e.g. MAX_INT
	}
	return _lastValue;
}



bool KSJet32Connection::writeIntRegister(uint32_t number, int32_t value) { 
	uint32_t comRef = 0x01020304;
    Jet32DataTelegram msg(_recPort);
//        msg.init(Jet32Command::readIntRegister, registerNumber, 0, comRef);
    msg.init(Jet32Command::writeVariable, number, value, comRef);

	if (!sendTelegramSynchron(&msg, comRef)) {
        LOGGER.printf("Write.IntegerReg %u with value %u failed\n", number, value);
		return false;
	}
	return true;
}



#endif      // #if defined USE_KSJet32Connection || defined KSLIBRARIES_USEALL