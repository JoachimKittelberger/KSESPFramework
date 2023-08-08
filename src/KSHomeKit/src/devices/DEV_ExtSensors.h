/**
 * @file DEV_ExtSensors.h
 *
 * @brief Header file for DEV_ExtSensors class
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



// use garageData in real Sketch
extern float currentTemperature;
extern float currentHumidity;
extern bool isCarPresent;


// A standalone Temperature sensor
struct DEV_ExtTempSensor : Service::TemperatureSensor {     

  // reference to the Current Temperature Characteristic
  SpanCharacteristic *temp;                                       

  // constructor() method
  DEV_ExtTempSensor() : Service::TemperatureSensor() {      

    // instantiate the Current Temperature Characteristic
    temp = new Characteristic::CurrentTemperature(-10.0);     
    // expand the range from the HAP default of 0-100 to -50 to 100 to allow for negative temperatures
    temp->setRange(-50, 100);                                 

    // initialization message
    Serial.print("Configuring Temperature Sensor");           
    Serial.print("\n");

  } // end constructor

  void loop() {

    // the temperature refreshes every 10 seconds by the elapsed time
    if (temp->timeVal() > SENSOR_LOOP_TIME) {
      // read temperature from sensor BME680
      temp->setVal(currentTemperature);
      LOG1("Temperature Update: ");
      LOG1(currentTemperature);
      LOG1(" ; ");
  }
  } // loop
};



////////////////////////////////////

// A standalone Humidity sensor
struct DEV_ExtHumSensor : Service::HumiditySensor {     

  // reference to the Current Humidity Characteristic
  SpanCharacteristic *hum;                                  

  // constructor() method
  DEV_ExtHumSensor() : Service::HumiditySensor() {      

    // instantiate the Current Temperature Characteristic
    hum = new Characteristic::CurrentRelativeHumidity(0);
    // expand the range to 30%-100% 
    hum->setRange(0, 100);                                

    // initialization message
    Serial.print("Configuring Humidity Sensor");           
    Serial.print("\n");

  } // end constructor

  void loop() {

    // the humidity refreshes every 10 seconds by the elapsed time
    if (hum->timeVal() > SENSOR_LOOP_TIME) {

      // set the new humidity; this generates an Event Notification and also resets the elapsed time        
      hum->setVal(currentHumidity);                            

      LOG1("Humidity Update: ");
      LOG1(currentHumidity);
      LOG1(" ; ");
    }
  } // loop
};



struct DEV_ExtOccupancySensor : Service::OccupancySensor {
  // reference to the Current Occupancy Characteristic
  SpanCharacteristic *occ;                                  

  DEV_ExtOccupancySensor() : Service::OccupancySensor() {
    
    // instantiate the Current Occupance Characteristic
    occ = new Characteristic::OccupancyDetected(0);

   
    // initialization message
    Serial.print("Configuring Occupancy Sensor");           
    Serial.print("\n");
  }

  void loop() {

    // the humidity refreshes every 10 seconds by the elapsed time
    if (occ->timeVal() > SENSOR_LOOP_TIME) {

      // set the new humidity; this generates an Event Notification and also resets the elapsed time        
      occ->setVal((uint8_t)isCarPresent);                            

      LOG1("Occupancy Update: ");
      LOG1((uint8_t)isCarPresent);
      LOG1("\n");
    }
  }
};

