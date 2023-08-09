/**
 * @file KSLogger.cpp
 * 
 * @brief Implementation file for using KSLogger
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

#if defined USE_KSLogger || defined KSLIBRARIES_USEALL     // include File in Build only if it ist defined to use it


#include "KSLogger.h"


// init the defined interface
bool KSSerialAndTelnet::init() {
	#if defined(LOG_SERIAL) || defined(LOG_SERIAL_AND_TELNET)
		Serial.begin(115200);
		#if ARDUINO_USB_CDC_ON_BOOT
			Serial.setTxTimeoutMs(0);
			vTaskDelay(pdMS_TO_TICKS(100));
		#else
			while (!Serial)
				yield();
		#endif
	#endif
	return true;
}



// implementations of abstract functions from Stream
int KSSerialAndTelnet::read() {
	return -1;
/*
	if (disconnected())
		return -1;
	return client.read();
	*/
}

int KSSerialAndTelnet::available() {
	return 0;
/*
	if (disconnected())
		return 0;
	return client.available();
	*/
}

int KSSerialAndTelnet::peek() {
	return -1;
	/*
	if (disconnected())
		return -1;
	return client.peek();
	*/
}


size_t KSSerialAndTelnet::write(uint8_t val) {
	if (_telnet)
		_telnet->write(val);
	return Serial.write(val);
}

size_t KSSerialAndTelnet::write(const uint8_t *buf, size_t size) {
	if (_telnet)
		_telnet->write(buf, size);
  	return Serial.write(buf, size);
}

void KSSerialAndTelnet::flush() {
	if (_telnet)
		_telnet->flush();
  	Serial.flush();
}


// instance
#if defined(USE_TELNET2)
	KSSerialAndTelnet SerialAndTelnet(&Telnet2);
#else
	KSSerialAndTelnet SerialAndTelnet(&Telnet);
#endif

#endif		// #if defined USE_KSLogger || defined KSLIBRARIES_USEALL
