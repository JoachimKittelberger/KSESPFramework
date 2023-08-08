/**
 * @file KSResetController.h
 * 
 * @brief Header file for using KSResetController
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

#include <ArduinoOTA.h>                             // http://arduino-esp8266.readthedocs.io/en/latest/ota_updates/readme.html


/* Example of Using:

// declaraion of object
KSResetController resetController(3*24*60*60);

// callback function
void onReset() {
  // is called before Reset of ESP32
  Serial.println("Reseting ESP32 ...");
  Serial.flush();
  vTaskDelay(pdMS_TO_TICKS(100));
}

// in setup()
  //resetController.setResetTime(3*24*60*60);
  resetController.setOnResetListener(onReset);
  resetController.create();

*/


// Reset the Controller after a defined period of time
// max unsigned long = 4,294,967,295 = 71582 min = 1193h = 49d

#define MAX_ULONG 4294967295
#define INIT_RESETTIME_IN_SEC (3 * 24 * 60 * 60)        // 3days


class KSResetController
{
    public:
        KSResetController(unsigned long resetTimeInSec = INIT_RESETTIME_IN_SEC);
        ~KSResetController();
  
        TaskHandle_t create();
        
        void setResetTime(unsigned long resetTimeInSec) {
            _resetTimeInMillis = resetTimeInSec * 1000;
        }
        static void reset();

        void setOnResetListener(void (*callback)()) { onResetListener = callback; }

    
    protected:
       void (*onResetListener)();

 
    private:
        void tKSResetController();
        TaskHandle_t _htKSResetController;

        unsigned long _resetTimeInMillis;
};
