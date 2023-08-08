/**
 * @file KSBattery.h
 * 
 * @brief Header for KSBattery class
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

#pragma once

#include "KSInterfaces/src/KSInterfaces.h"

#include "esp_adc_cal.h"


#define MAX_BATTERY_VOLTAGE 4.20
#define MIN_BATTERY_VOLTAGE 3.00

#define LOW_BATTERY_VOLTAGE 3.20
#define VERY_LOW_BATTERY_VOLTAGE 3.10
#define CRITICALLY_LOW_BATTERY_VOLTAGE 3.00

#define LOW_BATTERY_PERCENT 20
#define VERY_LOW_BATTERY_PERCENT 10
#define CRITICALLY_LOW_BATTERY_Percent 5


class KSBattery : public IKSBatteryPower {
    public:
        KSBattery(adc1_channel_t channel, float offset = 0.0);

        // implementation from IKSBatteryPower
        virtual float getVoltage();
        virtual float getBatteryPercentage() {
            return calculateBatteryPercentage(getVoltage(), _minVoltage, _maxVoltage);
        }

        static float calculateBatteryPercentage(float currentValue, float minVoltage = MIN_BATTERY_VOLTAGE, float maxVoltage = MAX_BATTERY_VOLTAGE);         // TODO: return float
        
        void setMinMaxVoltage(float minVoltage, float maxVoltage) { _minVoltage = minVoltage; _maxVoltage = maxVoltage; }

    private:
        adc1_channel_t _channel;
        float _offset = 0.0;            // offset to 4.2V MAX for example MOSFET in GND -> Diode +0.17V...

        // set min and max voltage for 0 and 100% and calculate percentage with this new window
        float _minVoltage = MIN_BATTERY_VOLTAGE;
        float _maxVoltage = MAX_BATTERY_VOLTAGE;

};



