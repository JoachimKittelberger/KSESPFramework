/**
 * @file DEV_ContactSensor.h
 *
 * @brief Header file for DEV_ContactSensor class
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

/////////////////////////////////
//   DEVICE-SPECIFIC SERVICES //
////////////////////////////////


#define SENSOR_LOOP_TIME 10000    // 10sec

enum HKContactState {
    CONTACT_DETECTED = 0,
    CONTACT_NOT_DETECTED = 1
};


const char *HKContactStates[] =
{
    "CONTACT_DETECTED",
    "CONTACT_NOT_DETECTED"
};


struct DEV_ContactSensor : Service::ContactSensor {
  // reference to the Current Occupancy Characteristic
  SpanCharacteristic *contact;                                  
  int _pinContact;
  bool _oldContactState = (bool)CONTACT_DETECTED;

  DEV_ContactSensor(int pinContact) : Service::ContactSensor() {

    _pinContact = pinContact;
    pinMode(_pinContact, INPUT_PULLDOWN);
  
    // instantiate the Current Occupance Characteristic
    contact = new Characteristic::ContactSensorState(CONTACT_DETECTED);
   
    // initialization message
    Serial.print("Configuring Contact Sensor");           
    Serial.print("\n");
  }

  void loop() {
    // if value has changed, set changed state imediately
    bool contactState = digitalRead(_pinContact);
    bool contactStateChanged = false;
    
    if (_oldContactState != contactState) {
      contactStateChanged = true;
      _oldContactState = contactState;
    }

    // the humidity refreshes every 10 seconds by the elapsed time
    if ((contact->timeVal() > SENSOR_LOOP_TIME) || contactStateChanged) {

      // set the new contact sensor state; this generates an Event Notification and also resets the elapsed time        
      contact->setVal((uint8_t)!contactState);  
      contactStateChanged = false;                          

      LOG1("Contact sensor Update: ");
      LOG1(HKContactStates[(uint8_t)contact->getVal()]);
      LOG1("\n");
    }
  }
};


