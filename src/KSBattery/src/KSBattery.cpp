/**
 * @file KSBattery.cpp
 * 
 * @brief Implementation for KSBattery class
 * 
 * @details handles communication to fritz.box homeautomation-switches
 * 
 * @see
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

#if defined USE_KSBattery || defined KSLIBRARIES_USEALL    // include File in Build only if it ist defined to use it


#include "KSBattery.h"


KSBattery::KSBattery(adc1_channel_t channel, float offset) {
    _channel = channel;
	_offset = offset;			// Offset in case of MOSFET in GND-line
}


/******************************************************************************
Description.: reads the battery voltage through the voltage divider at GPIO36
              if the ESP32-E has calibration eFused those will be used.
              In comparison with a regular voltmeter the values of ESP32 and
              multimeter differ only about 0.05V
Input Value.: -
Return Value: battery voltage in volts

GPIO36 = ADC1_CHANNEL_0
GPIO34 = ADC1_CHANNEL_6
******************************************************************************/
float KSBattery::getVoltage() {
	uint32_t value = 0;
	int rounds = 11;

	// adc_vref_init() {
		esp_adc_cal_characteristics_t adc_chars;

		//battery voltage divided by 2 can be measured at GPIO36, which equals ADC1_CHANNEL_0
		adc1_config_width(ADC_WIDTH_BIT_12);
		adc1_config_channel_atten(_channel, ADC_ATTEN_DB_11);
		esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars);
#ifdef DEBUG_OUTPUT
		switch(val_type) {
		case ESP_ADC_CAL_VAL_EFUSE_TP:
			Serial.printf("Characterized using Two Point Value --> coeff_a:%umV coeff_b:%umV\n", adc_chars.coeff_a, adc_chars.coeff_b);
			break;
		case ESP_ADC_CAL_VAL_EFUSE_VREF:
			Serial.printf("Characterized using eFuse Vref (%d mV)\r\n", adc_chars.vref);
			break;
		default:
			Serial.printf("Characterized using Default Vref (%d mV)\r\n", 1100);
		}
#endif
	//}

	//to avoid noise, sample the pin several times and average the result
	for (int i = 1; i <= rounds; i++) {
		value += adc1_get_raw(_channel);
	}
	value /= (uint32_t)rounds;

	//due to the voltage divider (1M+1M) values must be multiplied by 2 and convert mV to V
	return (esp_adc_cal_raw_to_voltage(value, &adc_chars) * 2.0 / 1000.0) + _offset;
}



float KSBattery::calculateBatteryPercentage(float currentValue, float minVoltage, float maxVoltage) {
	if (currentValue >= maxVoltage) return 100;
	if (currentValue < minVoltage) return 0;

	float percent = (currentValue - minVoltage) / (maxVoltage - minVoltage) * 100.0;
	return percent;
}




#endif		// #if defined USE_KSBattery || defined KSLIBRARIES_USEALL