/**
 * @file DEV_Speaker.h
 *
 * @brief Header file for DEV_Speaker class
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
// DEVICE-SPECIFIC Speaker SERVICES //
////////////////////////////////////


////////////////////////////////////

struct DEV_Speaker : Service::Speaker {               // First we create a derived class from the HomeSpan Speaker Service

  SpanCharacteristic *mute;                        // reference to the mute Characteristic
  SpanCharacteristic *volume;                        // reference to the volume Characteristic

  // Next we define the constructor for DEV_LED.  Note that it takes one argument, ledPin,
  // which specifies the pin to which the LED is attached.
  
  DEV_Speaker() : Service::Speaker(){

    mute = new Characteristic::Mute(true);     
    volume = new Characteristic::Volume(0);     

    
    Serial.print("Configuring Speaker");   // initialization message
    Serial.print("\n");

  } // end constructor



  boolean update(){                              // update() method
    LOG1("Updating Speaker");
    LOG1(":  Current Mute=");
    LOG1(mute->getVal()?"true":"false");
    LOG1("  Current Volume=");
    LOG1(volume->getVal());
 
    if (mute->updated()){
      LOG1("  New Mute=");
      LOG1(mute->getNewVal()?"true":"false");
    }

    if (volume->updated()){
      LOG1("  New Volume=");
      LOG1(volume->getNewVal());
    } 

    LOG1("\n");
    

    // TODO-> IR-Codes ausgeben
    //ledPin->set(power->getNewVal()*level->getNewVal());    
   
    return(true);                               // return true
  } // update
};
      
