/**
 * @file DEV_DoorsWindows.h
 *
 * @brief Header file for DEV_DoorsWindows class
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
//   DEVICE-SPECIFIC WindowCovering SERVICES //
////////////////////////////////////



struct DEV_WindowShade : Service::WindowCovering {     // A motorized Window Shade with Hold Feature

  SpanCharacteristic *current;                     // reference to a "generic" Current Position Characteristic (used by a variety of different Service)
  SpanCharacteristic *target;                      // reference to a "generic" Target Position Characteristic (used by a variety of different Service)

  DEV_WindowShade() : Service::WindowCovering() {       // constructor() method
        
    current=new Characteristic::CurrentPosition(0);     // Window Shades have positions that range from 0 (fully lowered) to 100 (fully raised)
    
    target=new Characteristic::TargetPosition(0);       // Window Shades have positions that range from 0 (fully lowered) to 100 (fully raised)
    target->setRange(0,100,10);                         // set the allowable target-position range to 0-100 IN STEPS of 10
        
    Serial.print("Configuring Motorized Window Shade");   // initialization message
    Serial.print("\n");

  } // end constructor




  boolean update(){                              // update() method

    // The logic below is based on how HomeKit appears to operate in practice, which is NOT consistent with
    // HAP documentation.  In that document HomeKit seems to support fully opening or fully closing a window shade, with
    // an optional control to HOLD the window shade at a given in-between position while it is moving.

    // In practice, HomeKit does not appear to implement any form of a HOLD control button, even if you instantiate that
    // Characteristic.  Instead, HomeKit provides a full slider control, similar to the brightness control for a lightbulb,
    // that allows you to set the exact position of the window shade from 0-100%.  This obviates the need to any sort of HOLD button.
    // The resulting logic is also very simple:

    if (target->getNewVal() > current->getVal()) {      // if the target-position requested is greater than the current-position, simply log a "raise" message  
      LOG1("Raising Shade to target value: ");                      // ** there is nothing more to do - HomeKit keeps track of the current-position so knows raising is required
      LOG1(target->getNewVal());
      LOG1("\n");
    }
    else if (target->getNewVal() < current->getVal()) {      // if the target-position requested is less than the current-position, simply log a "raise" message  
      LOG1("Lowering Shade to target value: ");                     // ** there is nothing more to do - HomeKit keeps track of the current-position so knows lowering is required
      LOG1(target->getNewVal());
      LOG1("\n");
    }
        
    return true;                               // return true
  
  } // update

  void loop(){                                     // loop() method

    // Here we simulate a window shade that takes 5 seconds to move to its new target posiiton
    
    if (current->getVal() != target->getVal() && target->timeVal() > 5000) {          // if 5 seconds have elapsed since the target-position was last modified...
      current->setVal(target->getVal());                                        // ...set the current position to equal the target position
    }

    // Note there is no reason to send continuous updates of the current position to the HomeKit.  HomeKit does NOT display the
    // current position.  Rather, it simply compares the value of the current position to the value of target positon as set by the
    // the user in the Home App.  If it finds current and target positions are the same, it knows the shade is stopped.  Otherwise
    // it will report the shade is raising or lowering depending on whether the specified target state is greater or less than
    // the current state.

    // According to HAP, the Characteristic Position State is also required.  However, this seems duplicative and is NOT needed
    // at all given the way HomeKit uses current position.  HomeSpan will warn you if Position State is not defined (since it 
    // is technically required) but this works fine without it.
    
  } // loop
  
};