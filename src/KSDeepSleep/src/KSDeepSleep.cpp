/**
 * @file KSDeepSleep.cpp
 * 
 * @brief Implementation for KSDeepSleep class
 * 
 * @details
 * 
 * @see
 *   - Details: https://randomnerdtutorials.com/esp32-deep-sleep-arduino-ide-wake-up-sources/
 *
 * @author Joachim Kittelberger <jkittelberger@kibesoft.de>
 * @date 28.07.2023
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

#if defined USE_KSDeepSleep || defined KSLIBRARIES_USEALL     // include File in Build only if it ist defined to use it

#include "KSDeepSleep.h"

#include "KSLogger/src/KSLogger.h"


void KSDeepSleep::printWakeupReason() {
    esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();

    switch (wakeup_reason) {
        case ESP_SLEEP_WAKEUP_EXT0 : LOGGER.println("Wakeup caused by external signal using RTC_IO"); break;
        case ESP_SLEEP_WAKEUP_EXT1 : LOGGER.println("Wakeup caused by external signal using RTC_CNTL"); break;
        case ESP_SLEEP_WAKEUP_TIMER : LOGGER.println("Wakeup caused by timer"); break;
        case ESP_SLEEP_WAKEUP_TOUCHPAD : LOGGER.println("Wakeup caused by touchpad"); break;
        case ESP_SLEEP_WAKEUP_ULP : LOGGER.println("Wakeup caused by ULP program"); break;
        default : LOGGER.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason); break;
    }
}


bool KSDeepSleep::isWakeupReasonGPIO() {
	esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();
	if (wakeup_reason == ESP_SLEEP_WAKEUP_EXT0)
		return true;
	return false;
}


bool KSDeepSleep::isWakeupReasonReset() {
	esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();

    switch(wakeup_reason)
    {
        case ESP_SLEEP_WAKEUP_EXT0 : LOGGER.println("Wakeup caused by external signal using RTC_IO"); break;
        case ESP_SLEEP_WAKEUP_EXT1 : LOGGER.println("Wakeup caused by external signal using RTC_CNTL"); break;
        case ESP_SLEEP_WAKEUP_TIMER : LOGGER.println("Wakeup caused by timer"); break;
        case ESP_SLEEP_WAKEUP_TOUCHPAD : LOGGER.println("Wakeup caused by touchpad"); break;
        case ESP_SLEEP_WAKEUP_ULP : LOGGER.println("Wakeup caused by ULP program"); break;
        default : LOGGER.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason);
            return true;
            break;
    }
	return false;
}


void KSDeepSleep::setWakeupTimer(uint64_t time_in_us) {
    _lastWakeupTimer_in_us = time_in_us;

	esp_sleep_enable_timer_wakeup(time_in_us);
	//int time_in_sec = int(time_in_us / US2SECOND);
	//LOGGER.println("Setup ESP32 to sleep for every " + String(time_in_sec) + " Seconds");
}


void KSDeepSleep::init(uint64_t time_in_us, gpio_num_t gpio_num, int level) {

    setWakeupTimer(time_in_us);

    /*
    Next we decide what all peripherals to shut down/keep on
    By default, ESP32 will automatically power down the peripherals
    not needed by the wakeup source, but if you want to be a poweruser
    this is for you. Read in detail at the API docs
    http://esp-idf.readthedocs.io/en/latest/api-reference/system/deep_sleep.html
    Left the line commented as an example of how to configure peripherals.
    The line below turns off all RTC peripherals in deep sleep.
    */
    //esp_deep_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
    //LOGGER.println("Configured all RTC Peripherals to be powered down in sleep");

    // folgender Code muss vewendet werden, wenn RTC Memory an anderer Stelle ausgeschalten wird  
    //esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_SLOW_MEM, ESP_PD_OPTION_ON);
    //esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_FAST_MEM, ESP_PD_OPTION_ON);
    
    /*
    First we configure the wake up source
    We set our ESP32 to wake up for an external trigger.
    There are two types for ESP32, ext0 and ext1 .
    ext0 uses RTC_IO to wakeup thus requires RTC peripherals
    to be on while ext1 uses RTC Controller so doesnt need
    peripherals to be powered on.
    Note that using internal pullups/pulldowns also requires
    RTC peripherals to be turned on.
    */
    if (gpio_num < GPIO_NUM_MAX) {
        //rtc_gpio_hold_en(gpio_num);    // damit wird für den DeepSleep die eingestellte Konfiguration behalten
        esp_sleep_enable_ext0_wakeup(gpio_num, level); //1 = High, 0 = Low
    }

    //If you were to use ext1, you would use it like
    //esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK,ESP_EXT1_WAKEUP_ANY_HIGH);
}



bool KSDeepSleep::sleep() {
	// if we have a callback function registered, call them
    
    if (_disableBecauseOTAActive) {
        return false;
    }

	if (onBeforeDeepSleepListener) {
		onBeforeDeepSleepListener();
	}

	// In the case that no wake up sources were provided but deep sleep was started, it will sleep forever unless hardware reset occurs.
	esp_deep_sleep_start();
    return true;
}



void KSDeepSleep::disableBluetooth() {
	btStop();
	esp_bluedroid_disable();
	esp_bluedroid_deinit();
	esp_bt_controller_disable();
	esp_bt_controller_deinit(); 
}


void KSDeepSleep::disableADC() {
 	adc_power_release();  // dafür muss aber adc_power_acquire(); am Anfang gesetzt werden
}



void KSDeepSleep::enableADC() {
  	adc_power_acquire();
}


void KSDeepSleep::disableWiFi() {
	WiFi.disconnect(true);
	WiFi.mode(WIFI_OFF);
	WiFi.setSleep(true);
	esp_wifi_stop();
}


void KSDeepSleep::disableROMLogging() {
    esp_deep_sleep_disable_rom_logging();		// disable ROM-Logging after deepSleep
}


#endif      // #if defined USE_KSDeepSleep || defined KSLIBRARIES_USEALL