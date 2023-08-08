/**
 * @file DEV_GarageDoor.h
 *
 * @brief Header file for DEV_GarageDoor class
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

#pragma once

////////////////////////////////////
//   DEVICE-SPECIFIC Door SERVICES //
////////////////////////////////////

// https://github.com/washcroft/HttpGarageDoorController/blob/master/GarageDoorController.h

// current door state format: uint8; HAP section 9.30; 0 = opened, 1 = closed, 2 = opening, 3 = closing, 4 = stopped not open or closed
// target door state // format: uint8; HAP section 9.118; 0 = opened, 1 = closed
// ostruction // format: bool; HAP section 9.65; 0 = no obstruction, 1 = obstruction detected


extern bool moveGarageDoor;

// time for opening/closing garage door in seconds
#define OPEN_CLOSE_DURATION_SEC 22


enum HKDoorState {
      DOORSTATE_OPEN = 0,
      DOORSTATE_CLOSED = 1,
      DOORSTATE_OPENING = 2,
      DOORSTATE_CLOSING = 3,
      DOORSTATE_STOPPED = 4,
      DOORSTATE_UNKNOWN = 255
    };


const char *HKCurrentDoorStates[] = {
    "DOORSTATE_OPEN",
    "DOORSTATE_CLOSED",
    "DOORSTATE_OPENING",
    "DOORSTATE_CLOSING",
    "DOORSTATE_STOPPED"
};

const char *HKTargetDoorStates[] = {
    "DOORSTATE_OPEN",
    "DOORSTATE_CLOSED"
};


struct DEV_GarageDoor : Service::GarageDoorOpener {     // A Garage Door Opener

  SpanCharacteristic *currentCharacteristic;            // reference to the Current Door State Characteristic (specific to Garage Door Openers)
  SpanCharacteristic *targetCharacteristic;             // reference to the Target Door State Characteristic (specific to Garage Door Openers)  
  SpanCharacteristic *obstructionCharacteristic;        // reference to the Obstruction Detected Characteristic (specific to Garage Door Openers)

  int _pinPushButton;
  int _pinContactDoorClosed;
  uint32_t _lastMillis = 0;


  DEV_GarageDoor(int pinContactDoorClosed = 0, int pinPushButton = 0) : Service::GarageDoorOpener() {       // constructor() method

    _pinPushButton = pinPushButton;
//    if (_pinPushButton) {
//      pinMode(_pinPushButton, OUTPUT);
//      digitalWrite(_pinPushButton, LOW);
//    }
    _pinContactDoorClosed = pinContactDoorClosed;
//    if (_pinContactDoorClosed) {
//      pinMode(_pinContactDoorClosed, INPUT_PULLDOWN);
//    }

    // read current state
    int currentDoorStateClosed = isSensorStateClosed();   // initial value of 1 means closed
    currentCharacteristic = new Characteristic::CurrentDoorState(currentDoorStateClosed);              // initial value of 1 means closed
    targetCharacteristic = new Characteristic::TargetDoorState(currentDoorStateClosed);                // initial value of 1 means closed
    obstructionCharacteristic = new Characteristic::ObstructionDetected(false);   // initial value of false means NO obstruction is detected
    
    Serial.print("Configuring Garage Door Opener");   // initialization message
    Serial.print("\n");

  } // end constructor



  boolean isSensorStateClosed() {
    bool bRetVal = true;   // initial value of 1 means closed
    if (_pinContactDoorClosed) {
      bRetVal = digitalRead(_pinContactDoorClosed);
      //Serial.printf("Current door sensor state: %d\n", bRetVal);
    }
    return bRetVal;
  }



  // wird aufgerufen, wenn sich Werte geändert haben
  boolean update() {                              // update() method

    // see HAP Documentation for details on what each value represents
    Serial.printf("update target state: %s\n", HKCurrentDoorStates[targetCharacteristic->getNewVal()]);

    if (targetCharacteristic->getNewVal() == DOORSTATE_OPEN) {                 // if the target-state value is set to 0, HomeKit is requesting the door to be in open position

      // wenn aktuell schon in Bewegung, dann anhalten und neu starten
      if (currentCharacteristic->getVal() == DOORSTATE_OPENING) {       // dieser Fall kommt vermutlich nie vor!
        LOG1("New State DOORSTATE_OPEN while DOORSTATE_OPENING *******************************\n");
        currentCharacteristic->setVal((int)DOORSTATE_STOPPED);                        // set the current-state value to 2, which means "opening"
        obstructionCharacteristic->setVal(true);                                              // clear any prior obstruction detection

        // reset output for Relais for Test. In practical set moveGarageDoor = true;
        moveGarageDoor = true;          // anhalten
        if (_pinPushButton) digitalWrite(_pinPushButton, LOW);
        return true;
      }
      if (currentCharacteristic->getVal() == DOORSTATE_CLOSING) {
        LOG1("New State DOORSTATE_OPEN while DOORSTATE_CLOSING\n");
        currentCharacteristic->setVal((int)DOORSTATE_STOPPED);                        // set the current-state value to 2, which means "opening"
        obstructionCharacteristic->setVal(true);                                              // clear any prior obstruction detection
        targetCharacteristic->setVal((int)DOORSTATE_CLOSED);                            // reverse target state, because door is now moving in other direction

        // reset output for Relais for Test. In practical set moveGarageDoor = true;
        moveGarageDoor = true;    // anhalten
        if (_pinPushButton) digitalWrite(_pinPushButton, LOW);
        return true;
      }
      if (currentCharacteristic->getVal() == DOORSTATE_STOPPED) {
        LOG1("New State DOORSTATE_OPEN while DOORSTATE_STOPPED\n");
      }

      
      LOG1("Opening Garage Door\n");
      currentCharacteristic->setVal((int)DOORSTATE_OPENING);                        // set the current-state value to 2, which means "opening"
      obstructionCharacteristic->setVal(false);                                              // clear any prior obstruction detection


      // set output for Relais for Test. In practical set moveGarageDoor = true;
      moveGarageDoor = true;
      if (_pinPushButton) digitalWrite(_pinPushButton, HIGH);
      _lastMillis = millis();

    } else if (targetCharacteristic->getNewVal() == DOORSTATE_CLOSED) {                 /// targetCharacteristic->getNewVal() == DOORSTATE_CLOSED

      // wenn aktuell schon in Bewegung, dann anhalten und neu starten
      if (currentCharacteristic->getVal() == DOORSTATE_OPENING) {
        LOG1("New State DOORSTATE_CLOSED while DOORSTATE_OPENING\n");
        currentCharacteristic->setVal((int)DOORSTATE_STOPPED);                        // set the current-state value to 2, which means "opening"
        obstructionCharacteristic->setVal(true);                                              // clear any prior obstruction detection
        targetCharacteristic->setVal((int)DOORSTATE_OPEN);                            // reverse target state, because door is now moving in other direction

        // reset output for Relais for Test. In practical set moveGarageDoor = true;
        moveGarageDoor = true;        // anhalten
        if (_pinPushButton) digitalWrite(_pinPushButton, LOW);
        return true;
      }
      if (currentCharacteristic->getVal() == DOORSTATE_CLOSING) {         // Dieser Fall kommt vermutlich nie vor
        LOG1("New State DOORSTATE_CLOSED while DOORSTATE_CLOSING *****************************\n");
        currentCharacteristic->setVal((int)DOORSTATE_STOPPED);                        // set the current-state value to 2, which means "opening"
        obstructionCharacteristic->setVal(true);                                              // clear any prior obstruction detection

        // reset output for Relais for Test. In practical set moveGarageDoor = true;
        moveGarageDoor = true;      // anhalten
        if (_pinPushButton) digitalWrite(_pinPushButton, LOW);
        return true;
     }
      if (currentCharacteristic->getVal() == DOORSTATE_STOPPED) {
        LOG1("New State DOORSTATE_CLOSED while DOORSTATE_STOPPED\n");
      }


      LOG1("Closing Garage Door\n");                                            // else the target-state value is set to 1, and HomeKit is requesting the door to be in the closed position
      currentCharacteristic->setVal((int)DOORSTATE_CLOSING);                         // set the current-state value to 3, which means "closing"         
      obstructionCharacteristic->setVal(false);                                 // clear any prior obstruction detection


      // set output for Relais for Test. In practical set moveGarageDoor = true;
      moveGarageDoor = true;
      if (_pinPushButton) digitalWrite(_pinPushButton, HIGH);
      _lastMillis = millis();
    }

    return true;                               // return true
  
  } // update





  void loop() {                                     // loop() method

    bool currentDoorStateClosed = true;
  
    if (_pinContactDoorClosed) {     // if we have a sensor for open state
      // check also manual changes in door state
      // read current state
      currentDoorStateClosed = isSensorStateClosed();   // initial value of 1 means closed
      //Serial.printf("Current door sensor state: %d\n", currentDoorStateClosed);

      // wenn alles offen, nichts machen
      if ((currentCharacteristic->getVal() == DOORSTATE_OPEN)
        && (targetCharacteristic->getVal() == DOORSTATE_OPEN)
        && !currentDoorStateClosed)
        return;

      // wenn alles geschlossen, nichts machen
      if ((currentCharacteristic->getVal() == DOORSTATE_CLOSED)
        && (targetCharacteristic->getVal() == DOORSTATE_CLOSED)
        && currentDoorStateClosed)
        return;

      // wenn geschlossen, aber sensor meldet offen
      if ((currentCharacteristic->getVal() == DOORSTATE_CLOSED)
        && !currentDoorStateClosed) {
        
        // zuerst targetstate und dann currentstate setzen
        targetCharacteristic->setVal((int)DOORSTATE_OPEN);
        currentCharacteristic->setVal((int)DOORSTATE_OPEN);           // set the current-state to the target-state
        Serial.println("Tor wurde von Hand geöffnet");
        return;
      }
      // wenn offen, aber sensor meldet geschlossen
      if ((currentCharacteristic->getVal() == DOORSTATE_OPEN)
        && currentDoorStateClosed) {
        
        // zuerst targetstate und dann currentstate setzen
        targetCharacteristic->setVal((int)DOORSTATE_CLOSED);
        currentCharacteristic->setVal((int)DOORSTATE_CLOSED);           // set the current-state to the target-state
        Serial.println("Tor wurde von Hand geschlossen");
        return;
      }
    } else {
      if (currentCharacteristic->getVal() == targetCharacteristic->getVal())        // if current-state matches target-state there is nothing do -- exit loop()
        return;
    }


    // if obstruction detected, do nothing
    if (obstructionCharacteristic->getVal())
      return;

    // if time OPEN_CLOSE_DURATION_SEC is over, stop moving
    if (millis() > _lastMillis + OPEN_CLOSE_DURATION_SEC * 1000) {
      if ((currentCharacteristic->getVal() != DOORSTATE_OPEN)
        && (currentCharacteristic->getVal() != DOORSTATE_CLOSED)) {
          currentCharacteristic->setVal(targetCharacteristic->getVal());           // set the current-state to the target-state
          
          // reset output for Relais for Test. In practical set moveGarageDoor = false; muss aber nicht gemacht werden, da dies im Hauptprogramm schon gemacht wird
          //moveGarageDoor = false;
          if (_pinPushButton) digitalWrite(_pinPushButton, LOW);
          
          Serial.println("Schalte Torantrieb aus.");
      }
    }
  } // loop
  
};

