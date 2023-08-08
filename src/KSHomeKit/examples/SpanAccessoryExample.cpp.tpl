
#include "HomeSpan.h" 

#include "devices/DEV_Identify.h"   
#include "devices/DEV_GarageDoor.h"
#include "devices/DEV_ExtSensors.h"
#include "devices/DEV_LED.h"     
#include "devices/DEV_DoorsWindows.h"
#include "devices/DEV_Blinker.h"
#include "devices/DEV_LEDwButton.h"
#include "devices/DEV_ProgButton.h"
#include "devices/DEV_Speaker.h"
#include "devices/DEV_Sensors.h"


void onSetupHomeSpanGarageDoor() {

  // no name in begin. Use setHostNameSuffix instead
  homeSpan.begin(Category::GarageDoorOpeners, "Garagentor", "Garage", hkManufacturer);   // initializes a HomeSpan device named "HomeSpan Lightbulb" with Category set to Lighting

  // vereinfachte Variante:
  new SpanAccessory();                                                          
    new DEV_Identify("Garagentor", hkManufacturer, "KS-00001", "Garage Door", SW_VERSION, 0);
    //new Service::HAPProtocolInformation();
      //new Characteristic::Version(HAPProtoInfoVersion);
     //(new DEV_GarageDoor(18, 19))->setPrimary();     
    (new DEV_GarageDoor(0, 19))->setPrimary();       // using with no open sensor
    // TODO*******************

  // create sensor for garage door state
  new SpanAccessory();
    new DEV_Identify("Garagentor geschlossen", hkManufacturer, "KS-00001", "Sensor", SW_VERSION, 0);
    //new Service::HAPProtocolInformation();          // Create the HAP Protcol Information Service  
      //new Characteristic::Version(HAPProtoInfoVersion);           // Set the Version Characteristicto "1.1.0" as required by HAP
    new DEV_ContactSensor(18);

   // create sensors for humidity and temperature
  new SpanAccessory();                                                          
    new DEV_Identify("Temperatur",hkManufacturer, "KS-00001","Sensor", SW_VERSION, 0);
    //new Service::HAPProtocolInformation();          // Create the HAP Protcol Information Service  
      //new Characteristic::Version(HAPProtoInfoVersion);           // Set the Version Characteristicto "1.1.0" as required by HAP
    new DEV_ExtTempSensor(); 

  new SpanAccessory();
    new DEV_Identify("Luftfeuchtigkeit",hkManufacturer, "KS-00001","Sensor", SW_VERSION, 0);
    //new Service::HAPProtocolInformation();          // Create the HAP Protcol Information Service  
      //new Characteristic::Version(HAPProtoInfoVersion);           // Set the Version Characteristicto "1.1.0" as required by HAP
    new DEV_ExtHumSensor();

  new SpanAccessory();
    new DEV_Identify("Auto in Garage",hkManufacturer, "KS-00001","Sensor", SW_VERSION, 0);
    //new Service::HAPProtocolInformation();          // Create the HAP Protcol Information Service  
      //new Characteristic::Version(HAPProtoInfoVersion);           // Set the Version Characteristicto "1.1.0" as required by HAP
    new DEV_ExtOccupancySensor();
    
}






void setupHomeSpan() {

  // The HomeSpan library creates a global object named "homeSpan" that encapsulates all HomeSpan functionality.
  // The begin() method is used to initialize HomeSpan and start all HomeSpan processes.
  
  // The first two parameters are Category and Name, which are used by HomeKit to configure the icon and name of the device shown in your Home App
  // when initially pairing your device.

  homeSpan.begin(Category::Bridges,"KibeSoft Bridge", "KibeSoft", "ESP-DevKitC");       // CHANGED!  Note that we replaced Category::Lighting with Bridges (this changes the icon when pairing)
  //homeSpan.begin(Category::Lighting, "KibeSoft LED", "KibeSoft", "ESP-DevKitC");   // initializes a HomeSpan device named "HomeSpan Lightbulb" with Category set to Lighting

  
  new SpanAccessory();  
    //new DEV_Identify("Bridge #1", "KibeSoft", "KS-00000", "KS Bridge", "0.9", 3);
    new DEV_Identify("Bridge #1", "KibeSoft", "KS-00000", "KS Bridge", "0.9", 0);
    //new Service::HAPProtocolInformation();
      //new Characteristic::Version("1.1.0");

  // Now we simply repeat the definitions of the previous LED Accessories, as per Example 7, with two exceptions:
  // 1) We no longer need to include the HAPProtocolInformation Service.
  // 2) We will set the number of blinks to zero, so that only the bridge accessory will cause the Built-In
  //    LED to blink. This becomes especially important if you had 20 Accessories defined and needed to wait a
  //    minute or more for all the blinking to finish while pairing.


  // Next, we construct a simple HAP Accessory Database with a single Accessory containing X Services,
  // each with their own required Characteristics.
  

// vereinfachte Variante:
  new SpanAccessory();                            // Begin by creating a new Accessory using SpanAccessory(), which takes no arguments 
    new DEV_Identify("LED", "KibeSoft", "KS-00001", "LED on/off", "0.9", 0);
//    new Service::HAPProtocolInformation();          // Create the HAP Protcol Information Service  
//      new Characteristic::Version("1.1.0");           // Set the Version Characteristicto "1.1.0" as required by HAP
     new DEV_LED(16);        // this instantiates a new LED Service.  Where is this defined?  What happpened to Characteristic::On?  Keep reading...



  // vereinfachte Variante:
  new SpanAccessory();                            // Begin by creating a new Accessory using SpanAccessory(), which takes no arguments 
    new DEV_Identify("Dim LED", "KibeSoft", "KS-00002", "LED dimmable", "0.9", 0);
//    new Service::HAPProtocolInformation();          // Create the HAP Protcol Information Service  
//      new Characteristic::Version("1.1.0");           // Set the Version Characteristicto "1.1.0" as required by HAP
    new DEV_DimmableLED(17);        // this instantiates a new LED Service.  Where is this defined?  What happpened to Characteristic::On?  Keep reading...
    // hiermit kann mit dem Taster die LED lokal ein-/ausgeschaltet werden.
    //new DEV_DimmableLEDwButton(17,13,0,0);          // NEW! added three extra arguments to specify the pin numbers for three SpanButtons() - see DEV_LED.h



  
  new SpanAccessory();                                                          
    new DEV_Identify("RGB LED", "KibeSoft", "KS-00003", "LED RGB", "0.9",0);
//    new Service::HAPProtocolInformation();
//      new Characteristic::Version("1.1.0");
    (new DEV_RgbLED(32,22,23))->setPrimary();                    // Create an RGB LED attached to pins 32,22,23 (for R, G, and B LED anodes)


  new SpanAccessory();                                                          
    new DEV_Identify("Garage Door","KibeSoft","KS-00001","Door","0.9",0);
//    new Service::HAPProtocolInformation();
//      new Characteristic::Version("1.1.0");
    new DEV_GarageDoor();                                                               // Create a Garage Door Opener (see DEV_DoorsWindows.h for definition)


  new SpanAccessory();                                                          
    new DEV_Identify("Window Shade","KibeSoft","KS-00001","Shade","0.9",0);
//    new Service::HAPProtocolInformation();
//      new Characteristic::Version("1.1.0");
    new DEV_WindowShade();                                                              // Create a motorized Window Shade (see DEV_DoorsWindows.h for definition)


  new SpanAccessory();                                                          
    new DEV_Identify("LED Blinker","KibeSoft","KS-00001","Volume Up CMD","0.9",0);
//    new Service::HAPProtocolInformation();
//      new Characteristic::Version("1.1.0");
    new DEV_Blinker(5, 3);                                                      // DEV_Blinker takes two arguments - pin, and number of times to blink




  new SpanAccessory();                                                          
    new DEV_Identify("PushButton Switches", "KibeSoft", "KS-00001", "Prog Switches", "0.9",0);

    // We've written DEV_ProgButton to take two arguments.  The first is a pin number that DEV_ProgButton.h uses to create a SpanButton.  The second is an index number
    // that HomeKit uses as a label when you program the actions of each button in the Home App.  The numbers do not have to be sequential, nor start with 1.  They just need
    // to be unique so HomeKit can distinguish them.  Note that HomeKit does not require index numbers if you only have one Stateless Programmable Switch Service within any
    // given Accessory.  Since we have two, we must specify two unique index numbers.
    
    new DEV_ProgButton(13,1);       // create Stateless Programmable Switch Service on pin 23 with index=1
    //new DEV_ProgButton(5,2);        // create Stateless Programmable Switch Service on pin 5 with index=2
 

  new SpanAccessory();                                                          
    new DEV_Identify("Speaker", "KibeSoft", "KS-00001", "Speaker Switches", "0.9",0);

    new DEV_Speaker();



  // create sensors for humidity and temperature
  new SpanAccessory();                                                          
    new DEV_Identify("DHT22 Temp Sensor","KibeSoft","KS-00001","Sensor","0.9",0);
    // Create a Temperature Sensor (see DEV_Sensors.h for definition)
    new DEV_TempSensor(); 
        

  new SpanAccessory();
    new DEV_Identify("DHT22 Humidity Sensor","KibeSoft","KS-00001","Sensor","0.9",0);
    // Create a Humidity Sensor (see DEV_Sensors.h for definition)
    new DEV_HumSensor();



}



void setupHomeSpanLED() {

  homeSpan.begin(Category::Lighting, "KibeSoft LED", "KibeSoft", "ESP-DevKitC");   // initializes a HomeSpan device named "HomeSpan Lightbulb" with Category set to Lighting

  // vereinfachte Variante:
  new SpanAccessory();                            // Begin by creating a new Accessory using SpanAccessory(), which takes no arguments 
    new DEV_Identify("LED", "KibeSoft", "KS-00001", "LED on/off", "0.9", 0);
    //new Service::HAPProtocolInformation();          // Create the HAP Protcol Information Service  
      //new Characteristic::Version("1.1.0");           // Set the Version Characteristicto "1.1.0" as required by HAP
      new DEV_LED(16);        // this instantiates a new LED Service.  Where is this defined?  What happpened to Characteristic::On?  Keep reading...
}





void setupHomeSpanSensors() {
  homeSpan.begin(Category::Bridges,"KibeSoft Sensor Bridge", "KibeSoft", "ESP-DevKitC");       // CHANGED!  Note that we replaced Category::Lighting with Bridges (this changes the icon when pairing)

  new SpanAccessory();  
    new DEV_Identify("Bridge #1", "KibeSoft", "KS-00000", "KS Sensor Bridge", "0.9", 0);
    //new Service::HAPProtocolInformation();
      //new Characteristic::Version("1.1.0");

  // create sensors for humidity and temperature
  new SpanAccessory();                                                          
    new DEV_Identify("BME680 Temp Sensor","KibeSoft","KS-00001","Sensor","0.9",0);
    // Create a Temperature Sensor (see DEV_Sensors.h for definition)
    new DEV_TempSensor(); 

  new SpanAccessory();
    new DEV_Identify("BME680 Humidity Sensor","KibeSoft","KS-00001","Sensor","0.9",0);
    // Create a Humidity Sensor (see DEV_Sensors.h for definition)
    new DEV_HumSensor();

  new SpanAccessory();
    new DEV_Identify("BME680 CO2 Sensor","KibeSoft","KS-00001","Sensor","0.9",0);
    // Create a Humidity Sensor (see DEV_Sensors.h for definition)
    new DEV_CO2Sensor();

  new SpanAccessory();
    new DEV_Identify("GY302 Illumincance Sensor","KibeSoft","KS-00001","Sensor","0.9",0);
    // Create a Humidity Sensor (see DEV_Sensors.h for definition)
    new DEV_LightSensor();
}


