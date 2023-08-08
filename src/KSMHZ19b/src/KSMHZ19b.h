/**
 * @file KSMHZ19b.h
 * 
 * @brief Header file for using KSMHZ19b class
 * 
 * @details 
 * 
 * @see
 *   - used library: https://github.com/WifWaf/MH-Z19.git
 *   - https://github.com/plerup/espsoftwareserial.git
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

#ifndef _KSMHZ19B_H
#define _KSMHZ19B_H



#include <MHZ19.h>
#include <SoftwareSerial.h>


#define MHZ19B_RX_PIN 26                                          // Rx pin which the MHZ19 Tx pin is attached to
#define MHZ19B_TX_PIN 25                                          // Tx pin which the MHZ19 Rx pin is attached to
#define MHZ19B_BAUDRATE 9600                                      // Device to MH-Z19 Serial baudrate (should not be changed)


class KSMHZ19b
{
    public:
        KSMHZ19b(int8_t RXPin, uint8_t TXPin);
        ~KSMHZ19b() {};

        void init();

        int readCO2();
        bool read(int* pCO2 = NULL);

    private:
        MHZ19 _mhz19b;
        SoftwareSerial _serial;       								// create device to MH-Z19 serial
        //HardwareSerial _serial(1);                              	// (ESP32 Example) create device to MH-Z19 serial
        int8_t _RXPin = MHZ19B_RX_PIN;
        int8_t _TXPin = MHZ19B_TX_PIN;

};


#endif  // #define _KSMHZ19B_H