/**
 * @file DEV_Sensors.h
 *
 * @brief Header file for DEV_Sensors class
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


#include "KSBME680BSec/src/KSBME680BSec.h"
#include "KSGY302/src/KSGY302.h"
#include "KSCriticalSection/src/KSCriticalSection.h"

#define SENSOR_LOOP_TIME 10000    // 10sec

KSCriticalSection csI2CSensor;			// Critcal Section for access to I2C

KSGY302 gy302(LOW, &csI2CSensor);
#define BME68X_DEFAULT_ADDRESS 0x77
KSBME680BSec bme680(BME68X_DEFAULT_ADDRESS, &Wire, &csI2CSensor);

float temperature;
float humidity;
float co2Equivalent;
float illuminance;

bool bSensorError;


bool getBME680Readings() {

   if (!bme680.iaqSensor.run()) { // If no data is available
        bme680.checkIaqSensorStatus();
        //Serial.println("bme680: No data available");
        bSensorError = true;
        return false;
    }
    bSensorError = false;

    illuminance = gy302.readIlluminance();
    temperature = bme680.iaqSensor.temperature;
    humidity = bme680.iaqSensor.humidity;
    co2Equivalent = bme680.iaqSensor.co2Equivalent;

/*
    Serial.print("Illuminance = "); Serial.print(illuminance); Serial.println(" lux");
    Serial.print("Temperature = "); Serial.print(bme680.iaqSensor.temperature); Serial.println(" °C");
    Serial.print("Pressure = "); Serial.print(bme680.iaqSensor.pressure / 100.0); Serial.println(" hPa");
    Serial.print("Humidity = "); Serial.print(bme680.iaqSensor.humidity); Serial.println(" %");
    Serial.print("IAQ = "); Serial.print(bme680.iaqSensor.staticIaq); Serial.println("");
    Serial.print("CO2 equiv = "); Serial.print(bme680.iaqSensor.co2Equivalent); Serial.println("");
    Serial.print("Breath VOC = "); Serial.print(bme680.iaqSensor.breathVocEquivalent); Serial.println("");
    Serial.println();
*/
    return true;
}



// A standalone Temperature sensor
struct DEV_TempSensor : Service::TemperatureSensor {     

  // reference to the Current Temperature Characteristic
  SpanCharacteristic *temp;                                       

  // constructor() method
  DEV_TempSensor() : Service::TemperatureSensor() {      

    // init only in TempSensor not in each sensor
    Wire.begin();
    bme680.init();			      // setup BME280
    bSensorError = true;      // use error for all sensors. We just want to read bmeSettings once for all sensors


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
      if (getBME680Readings()) {
        // set the new temperature; this generates an Event Notification and also resets the elapsed time
        temp->setVal(temperature);                            

        LOG1("Temperature Update: ");
        LOG1(temperature);
        LOG1(" ; ");
      }
    }
  } // loop
};

////////////////////////////////////

// A standalone Humidity sensor
struct DEV_HumSensor : Service::HumiditySensor {     

  // reference to the Current Humidity Characteristic
  SpanCharacteristic *hum;                                  

  // constructor() method
  DEV_HumSensor() : Service::HumiditySensor() {      

    //already done in tempsensor
    //Wire.begin();
    //bme680.init();			// setup BME280

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
      // read humidity from sensor BME680
      if (!bSensorError) {
      //if (getBME680Readings()) {
        // set the new humidity; this generates an Event Notification and also resets the elapsed time        
        hum->setVal(humidity);                            

        LOG1("Humidity Update: ");
        LOG1(humidity);
        LOG1(" ; ");
      }
    }
  } // loop
};



////////////////////////////////////

// A standalone CO2 sensor
struct DEV_CO2Sensor : Service::CarbonDioxideSensor {     

  // reference to the Current CarbonDioxide Characteristic
  SpanCharacteristic *co2;                                  
  SpanCharacteristic *co2Level;                                  
  SpanCharacteristic *co2PeakLevel;

  float co2MaxValue = 0;                                  

  // constructor() method
  DEV_CO2Sensor() : Service::CarbonDioxideSensor() {      

    //already done in tempsensor
    //Wire.begin();
    //bme680.init();			// setup BME280

    // instantiate the Current CO2 Characteristic
    co2 = new Characteristic::CarbonDioxideDetected(false);
    // expand the range to 30%-100% 
    //co2->setRange(30, 100);                                

    co2Level = new Characteristic::CarbonDioxideLevel(500);
    co2PeakLevel = new Characteristic::CarbonDioxidePeakLevel(500);
 
    // initialization message
    Serial.print("Configuring CO2 Sensor");           
    Serial.print("\n");

  } // end constructor

  void loop() {

    // the CO2 refreshes every 10 seconds by the elapsed time
    if (co2Level->timeVal() > SENSOR_LOOP_TIME) {
      // read humidCO2ity from sensor BME680
      if (!bSensorError) {
      //if (getBME680Readings()) {
        // set the new humidity; this generates an Event Notification and also resets the elapsed time        
        co2Level->setVal(co2Equivalent);

        if (co2Equivalent > co2MaxValue) {
          co2MaxValue = co2Equivalent;
          co2PeakLevel-> setVal(co2MaxValue);
        }

        if (co2Equivalent > 1200)
          co2->setVal(true);
        else
          co2->setVal(false);

        LOG1("CO2 Update: ");
        LOG1(co2Equivalent);
        LOG1(" ; Detected: ");
        LOG1(co2Equivalent > 4000 ? "true ; " : "false ; ");
      }
    }
  } // loop
};



////////////////////////////////////

// A standalone illuminance sensor
struct DEV_LightSensor : Service::LightSensor {     

  // reference to the Current LightSensor Characteristic
  SpanCharacteristic *light;                                  

  // constructor() method
  DEV_LightSensor() : Service::LightSensor() {      

    //already done in tempsensor
    //Wire.begin();
    //bme680.init();			// setup BME280

    // instantiate the Current illuminance Characteristic
    light = new Characteristic::CurrentAmbientLightLevel(1);
    // expand the range to 30%-100% 
    //light->setRange(0, 10000);                                

    // initialization message
    Serial.print("Configuring Illuminance Sensor");           
    Serial.print("\n");

  } // end constructor

  void loop() {

    // the illuminance refreshes every 10 seconds by the elapsed time
    if (light->timeVal() > SENSOR_LOOP_TIME) {
      // read illuminance from sensor GY302
      if (!bSensorError) {
      //if (getBME680Readings()) {
        // set the new illuminance; this generates an Event Notification and also resets the elapsed time        
        light->setVal(illuminance);                            

        LOG1("Illuminance Update: ");
        LOG1(illuminance);
        LOG1("\n");
      }
    }
  } // loop
};



