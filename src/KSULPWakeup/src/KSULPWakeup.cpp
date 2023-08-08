/**
 * @file KSULPWakeup.cpp
 * 
 * @brief Implementation of KSULPWakeup functions
 * 
 * @details
 * 
 * @see
 * 
 * @author Joachim Kittelberger <jkittelberger@kibesoft.de>
 * @date 24.04.2022
 * @version 1.01
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

#if defined USE_KSULPWakeup || defined KSLIBRARIES_USEALL     // include File in Build only if it ist defined to use it

#include "KSULPWakeup.h"


#include <esp32/rom/rtc.h>
#include <soc/rtc_cntl_reg.h>
#include <soc/rtc_io_reg.h>
#include <soc/timer_group_reg.h>
#include <soc/uart_reg.h>



//TODO
// (X) Header-Files für ROM-Funktionen unter: https://github.com/espressif/esp-idf/tree/master/components/esp_rom/include/esp32/rom
// Beim Bootloader die CONFIG_BOOTLOADER_SKIP_VALIDATE_IN_DEEP_SLEEP Option aktivieren unter Arduino?
// Ändern von Systemvariablen in der SDK-Configuration https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/kconfig.html
// dabei geht es um "Skip image validation when exiting deep sleep" in der Bootloader config




// da wir nicht so genau sein müssen, benötigen wir das hier nicht
// hier wird die länge der ULP-Prozessorzeit abgezogen. Könnte gemessen werden und dann hier angepasst werden
/*
#define USE_EXTERNAL_RTC_CRYSTAL		// Comment out this line if you're using the internal RTC RC (150KHz) oscillator.
#ifdef USE_EXTERNAL_RTC_CRYSTAL
	#define DEEP_SLEEP_TIME_OVERHEAD_US (650 + 100 * 240 / CONFIG_ESP32_DEFAULT_CPU_FREQ_MHZ)
#else
	#define DEEP_SLEEP_TIME_OVERHEAD_US (250 + 100 * 240 / CONFIG_ESP32_DEFAULT_CPU_FREQ_MHZ)
#endif // USE_EXTERNAL_RTC_CRYSTAL
*/
// setzt die WakeupTime in us für den deepSleep
RTC_IRAM_ATTR void ulpSetDeepSleepWakeupTimerForUs(uint64_t duration_us) {
	REG_WRITE(TIMG_WDTFEED_REG(0), 1);    		// feed the watchdog
	
	// Get RTC calibration
	uint32_t period = REG_READ(RTC_SLOW_CLK_CAL_REG);
	// Calculate sleep duration in microseconds
	//int64_t sleep_duration = (int64_t)duration_us - (int64_t)DEEP_SLEEP_TIME_OVERHEAD_US; // Da wir nicht so genau sein müssen, benötigen wir das nicht.
	int64_t sleep_duration = (int64_t)duration_us;
	if (sleep_duration < 0) {
		sleep_duration = 0;
	}
	// Convert microseconds to RTC clock cycles
	int64_t rtc_count_delta = (sleep_duration << RTC_CLK_CAL_FRACT) / period;

	REG_WRITE(TIMG_WDTFEED_REG(0), 1);		    // feed the watchdog
	uint64_t now = ulpRtcTimeGetClockCycles();

	// Set wakeup time
	uint64_t future = now + rtc_count_delta;
	WRITE_PERI_REG(RTC_CNTL_SLP_TIMER0_REG, future & UINT32_MAX);
	WRITE_PERI_REG(RTC_CNTL_SLP_TIMER1_REG, future >> 32);
	//rtc_cntl_ll_set_wakeup_timer(future); // can be called because it is static inline (not in flash) instead of the 2 WRITE_PERI_REG above
}



// set the GPIO for external 0 Wakeup
RTC_IRAM_ATTR esp_err_t ulpSetExt0WakeupPin(gpio_num_t gpio_num, int level) {
	return esp_sleep_enable_ext0_wakeup(gpio_num, level);
}




// liefert die RTC-Clock cyles. Muss noch in us umgerechnet werden
RTC_IRAM_ATTR uint64_t ulpRtcTimeGetClockCycles(void) {
    SET_PERI_REG_MASK(RTC_CNTL_TIME_UPDATE_REG, RTC_CNTL_TIME_UPDATE);
    while (GET_PERI_REG_MASK(RTC_CNTL_TIME_UPDATE_REG, RTC_CNTL_TIME_VALID) == 0) {
        ets_delay_us(1); // might take 1 RTC slowclk period, don't flood RTC bus
    }
    SET_PERI_REG_MASK(RTC_CNTL_INT_CLR_REG, RTC_CNTL_TIME_VALID_INT_CLR);
    uint64_t timeNow = READ_PERI_REG(RTC_CNTL_TIME0_REG);
    timeNow |= ((uint64_t) READ_PERI_REG(RTC_CNTL_TIME1_REG)) << 32;
    return timeNow;
}



// liefert die aktuelle RTC-Zeit in us
RTC_IRAM_ATTR uint64_t ulpRtcTimeGetUs(void) {
	// Get RTC calibration
	uint32_t period = REG_READ(RTC_SLOW_CLK_CAL_REG);

	// Convert RTC clock cycles to microseconds
 	uint64_t timeNow = (ulpRtcTimeGetClockCycles() * period) >> RTC_CLK_CAL_FRACT;
	return timeNow;
}



// Set the pointer of the wakeup stub function.
RTC_IRAM_ATTR void ulpSetWakeupStub(void (*wakeupStubfunc)()) {
	REG_WRITE(RTC_ENTRY_ADDR_REG, (uint32_t)wakeupStubfunc);
}



// setzt den ULP wieder in den deepsleep mit den übergebenen wakeupMask
RTC_IRAM_ATTR void ulpStartDeepSleep(uint32_t wakeupMask) {

	REG_SET_FIELD(RTC_CNTL_WAKEUP_STATE_REG, RTC_CNTL_WAKEUP_ENA, wakeupMask);
	WRITE_PERI_REG(RTC_CNTL_SLP_REJECT_CONF_REG, 0);                                  // Clear sleep rejection cause

	// Go to sleep.
	CLEAR_PERI_REG_MASK(RTC_CNTL_STATE0_REG, RTC_CNTL_SLEEP_EN);
	SET_PERI_REG_MASK(RTC_CNTL_STATE0_REG, RTC_CNTL_SLEEP_EN);
	// A few CPU cycles may be necessary for the sleep to start...
	while (true) {
		;
	}
	// never reaches here.
	// go to deepsleep
}



// feed the watchdog
RTC_IRAM_ATTR void ulpFeedWatchdog(void) {
	REG_WRITE(TIMG_WDTFEED_REG(0), 1);								// feed the watchdog
}



// wait for UART transmitting
RTC_IRAM_ATTR void ulpWaitUARTTXEnd(uint32_t uartNumber) {
    while (REG_GET_FIELD(UART_STATUS_REG(0), UART_ST_UTX_OUT)) {
    }	// Wait for UART to end transmitting.
}



// reads the digital pin
RTC_IRAM_ATTR bool ulpDigitalRead(uint8_t pin) {
    return GPIO_INPUT_GET(pin) > 0;
}




#endif		// #if defined USE_KSULPWakeup || defined KSLIBRARIES_USEALL