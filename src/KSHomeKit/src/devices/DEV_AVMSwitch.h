/**
 * @file DEV_AVMSwitch.h
 *
 * @brief Header file for DEV_AVMSwitch class
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
//   DEVICE-SPECIFIC Switcht SERVICES //
////////////////////////////////////

#include "KSAVMConnection/src/KSAVMConnection.h"


const uint32_t WAITMS_FOR_PRESENCE = 60000;      // wait x milli seconds til check presence of switch
const uint32_t WAITMS_FOR_NEWSTATE = 10000;      // wait x milli seconds til chek new state

struct DEV_AVMSwitch : Service::Switch {
  
  SpanCharacteristic *power;                        // reference to the On Characteristic
  SpanCharacteristic *temp;                        // reference to the On Characteristic for temperature
  
  String _ain;
  KSAVMConnection* _fbConnection;
  uint32_t _lastMillis;
  uint32_t _waitOffset;

  bool _currentSwitchState;


  DEV_AVMSwitch(const String AIN, KSAVMConnection* fbConnection) : Service::Switch() {

    _ain = AIN;
    _fbConnection = fbConnection;
    //_lastMillis = millis();
    _lastMillis = 0;
    _waitOffset = WAITMS_FOR_NEWSTATE;
    //_waitOffset = WAITMS_FOR_PRESENCE;

    //_fbConnection = NULL;     // TODO just for test

    power = new Characteristic::On();     

    // set initial value    -> this leads to hanging Wifi-task
    //if (_fbConnection) power->setVal(_fbConnection->GetSwitchState(_ain));
    power->setVal(false);
  }
  
  
  boolean update(){                              // update() method

    LOG1("Updating Switch ");
    LOG1(_ain.c_str());
    LOG1(":  Current Power = ");
    LOG1(power->getVal() ? "true" : "false");
  
    //if (power->updated()){
      LOG1("  New Power = ");
      LOG1(power->getNewVal() ? "true" : "false");
      LOG1("\n");
    //}

    // set the new state
    //(int)power->getNewVal();
    if (_fbConnection) {
      if (_fbConnection->isWIFIConnected()) {
        if (_fbConnection->IsSwitchPresent(_ain)) {
          _fbConnection->SetSwitchState(_ain, power->getNewVal() ? SwState::ON : SwState::OFF);
          power->setVal(power->getNewVal());
          return true;
        }
      }
    }
    //power->setVal(power->getNewVal());
    return false;                               // return true
  } // update



  void loop() {                                     // loop() method
    
    // check every x seconds the status of the devices
    if (millis() > _lastMillis + _waitOffset) {
      if (_fbConnection) {

        // if switch is not present, wait a longer time
        _currentSwitchState = _fbConnection->IsSwitchPresent(_ain);
        if (!_currentSwitchState) {
          _waitOffset = WAITMS_FOR_PRESENCE;
          _lastMillis = millis();
          return;
        }

        _waitOffset = WAITMS_FOR_NEWSTATE;
        _currentSwitchState = _fbConnection->GetSwitchState(_ain);
        if (_currentSwitchState != power->getVal()) {
          power->setVal(_currentSwitchState);
          Serial.printf("Switch %s changed from outside HomeKit to %s\n", _ain.c_str(), power->getVal() ? "true" : "false");
        }
      }
      _lastMillis = millis();
    }
  } // loop


};

