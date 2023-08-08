/**
 * @file KSInstaModul.h
 *
 * @brief Header file for KSInstaModul class
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

#ifndef _KSINSTAMODUL_H
#define _KSINSTAMODUL_H


// Das 2. Modul ist nicht wirklich defekt. Es scheint aus irgendwelchen Gründen aber nicht mit dem Dimmer im Bad zu funktionieren.
// mit dem Schaltaktor der Terrasse hat er fuktioniert

// Das Kurz-Telegramm muss nur einmal weggeschickt werden. Intern werden dann die entsprechenden Sequenzen für Funk erzeugt
// Beim Long-Telegramm muss das Start-Telegramm geschickt werden. Standardmässig wird dann nach 12 sec das Stopp-Telegramm geschickt. (Noch verifizieren)
// Wenn schon früher gestoppt werden soll, muss vorher ein Stopp-Telegramm geschickt werden

// Zum einlernen von neuen Modulen muss immer ein Long-Telegramm geschickt werden

// beim Modul muss ein Puffer-Kondensator eingebaut werden, damit die Stromversorgung nicht sporadisch zusammenbricht.
// Zeigt sich durch Reboots bzw. zusätzliche Zeichen im RX-Buffer, die dort nicht hingehören. Modul ist danach nicht mehr TX-fähig

// über Telnet in der Test-App können die Kommandos
// CmdA1+ bis CmdC7- für Kurztelegramm 
// CmdA1+L bis CmdC7-L für Langtelegramm (Dauer 2 sec)
// geschickt werden.



#include <HardwareSerial.h>
#include <Stream.h>

#include <ArduinoJson.h>


const char to_hex[16] =
{
    '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};


#define InstaBUFFER_SIZE_TELEGRAMM 12

// define the ChannelKeys
#define InstaChannel1Plus        0x40
#define InstaChannel1Minus       0x80
#define InstaChannel2Plus        0x41
#define InstaChannel2Minus       0x81
#define InstaChannel3Plus        0x42
#define InstaChannel3Minus       0x82
#define InstaChannel4Plus        0x43
#define InstaChannel4Minus       0x83
#define InstaChannel5Plus        0x44
#define InstaChannel5Minus       0x84
#define InstaChannel6Plus        0x45
#define InstaChannel6Minus       0x85
#define InstaChannel7Plus        0x46
#define InstaChannel7Minus       0x86
#define InstaChannel8Plus        0x47
#define InstaChannel8Minus       0x87

#define InstaLightScene1         0xD8
#define InstaLightScene2         0xD9
#define InstaLightScene3         0xDA
#define InstaLightScene4         0xDB
#define InstaLightScene5         0xDC
#define InstaMasterPlus          0x58        // hier gibt es beim Empfangen 5 Szenen!
#define InstaMasterMinus         0x98

// sending the last selected LightScene for dimm
#define InstaLightScene1Plus     0x58
#define InstaLightScene1Minus    0x98
#define InstaLightScene2Plus     0x59
#define InstaLightScene2Minus    0x99
#define InstaLightScene3Plus     0x5A
#define InstaLightScene3Minus    0x9A
#define InstaLightScene4Plus     0x5B
#define InstaLightScene4Minus    0x9B
#define InstaLightScene5Plus     0x5C
#define InstaLightScene5Minus    0x9C

#define InstaAllOn               0x5F
#define InstaAllOff              0x9E

// defines for group
#define InstaGroupA              0x00
#define InstaGroupB              0x08
#define InstaGroupC              0x10



// define the messages
#define InstaSHORTMESSAGE        0x01
#define InstaLONGMESSAGE         0x02
#define InstaSTOPMESSAGE         0x08


// define vars in cpp-File
//extern uint8_t InstaChannels[8][2];
//extern uint8_t InstaGroups[3];
extern uint8_t InstaChannels[][2];
extern uint8_t InstaGroups[];


class KSInstaModul
{
public:
//    KSInstaModul(const SoftwareSerial& serial);
    KSInstaModul(HardwareSerial& serial, int enablePin = -1) : _HWSerial(serial), _enablePin(enablePin), onNewMessage(NULL) {
        _mutexSerial = xSemaphoreCreateMutex();
    }

    bool sendShortTelegram(uint8_t key, uint8_t group);
    bool sendLongTelegram(uint8_t key, uint8_t group, int duration);

    bool getBoardFeatures();

    void setOnNewMessageListener(void (*callback)(uint8_t group, uint8_t channel, bool plus, bool switchOn)) {
        onNewMessage = callback;
    }

    TaskHandle_t createConnection();

    String generateRpcString(String src, uint8_t group, uint8_t channel, bool plus, bool switchOn);


protected:
    void begin();
    
    virtual int available()     { return _HWSerial.available(); }
    virtual int peek()          { return _HWSerial.peek(); }
    virtual int read()          { return _HWSerial.read(); }

    virtual void flush()        { return _HWSerial.flush(); }
    virtual size_t write(uint8_t ch) { return _HWSerial.write(ch); }
    //virtual size_t write(const char *str) { return _HWSerial.write(str); }
    virtual size_t write(const uint8_t *pBuffer, size_t size) { return _HWSerial.write(pBuffer, size); }

    int emptyRXQueue();
    

    virtual bool handleHandshakeRequest();
    virtual bool handleNewMessage();

    void processNewMessage(uint8_t* pBuffer);
        
    bool startHandshake();
    uint8_t calculateCRC(uint8_t* pBuffer);

    bool sendTelegram(bool useHandshake, uint8_t SW1, uint8_t SW2, uint8_t DB1, uint8_t DB2);
    void printTelegram(uint8_t* pBuffer);

private:
    void tKSInstaModul();
    TaskHandle_t _htKSInstaModul;
  
    void (*onNewMessage)(uint8_t group, uint8_t channel, bool plus, bool switchOn);
 
    volatile uint8_t _recTimeNotComplete;     // count the loops we need to get a complete message

    SemaphoreHandle_t _mutexSerial;

    HardwareSerial& _HWSerial;
    int _enablePin = -1;
};



#endif          // define _KSINSTAMODUL_H