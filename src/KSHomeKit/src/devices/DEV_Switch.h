/**
 * @file DEV_Switch.h
 *
 * @brief Header file for DEV_Switch class
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


struct DEV_Switch : Service::Switch {
  
  SpanCharacteristic *power;                        // reference to the On Characteristic
  
  int* _pSwitchCmd;     // cmd 1 = on, 0 = off
  int _id;
  int _pin;             // outputpin for Relais

  DEV_Switch(int id, int* pSwitchCmd = NULL, int pin = 0) : Service::Switch() {

    _pSwitchCmd = pSwitchCmd;
    _id = id;
    _pin = pin;

    power = new Characteristic::On();     


    // set initial value
    if (_pin) power->setVal(digitalRead(_pin));

    // wenn wir die lezten Werte sichern wollen, müssen wir diese bei der Initialisierung setzen
    //update();
  }
  
  
  boolean update(){                              // update() method

    LOG1("Updating Switch ");
    LOG1(_id);
    LOG1(":  Current Power = ");
    LOG1(power->getVal() ? "true" : "false");
  
    //if (power->updated()){
      LOG1("  New Power = ");
      LOG1(power->getNewVal() ? "true" : "false");
      LOG1("\n");
    //}

    // set the new state
    if (_pSwitchCmd)
      *_pSwitchCmd = (int)power->getNewVal();
    power->setVal(power->getNewVal());

    // Just For Test with HomeKitDemo *******************
    //digitalWrite(_pin, power->getNewVal());

    return true;                               // return true
  
  } // update



  void loop() {                                     // loop() method

    if (_pin) {
      // switch wurde von ausserhalb betätigt
      if ((digitalRead(_pin) != power->getVal()) && (power->timeVal() > 200)) {      // gib der Hautpschleife 200ms Zeit, um den Ausgang zu setzen/rücksetzen
        power->setVal(digitalRead(_pin));
        LOG1("Switch changed from outside HomeKit ");
        LOG1(_id);
        LOG1(":  Current Power = ");
        LOG1(power->getVal() ? "true" : "false");
        LOG1("\n");
      }
   }

  } // loop


};

