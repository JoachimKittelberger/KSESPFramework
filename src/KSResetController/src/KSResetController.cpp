/**
 * @file KSResetController.cpp
 * 
 * @brief Implementation of KSResetController
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

#if defined USE_KSResetController || defined KSLIBRARIES_USEALL     // include File in Build only if it ist defined to use it

#include "KSResetController.h"

#include "KSLogger/src/KSLogger.h"


KSResetController::KSResetController(unsigned long resetTimeInSec) {
	onResetListener = NULL;
	_resetTimeInMillis = resetTimeInSec * 1000;
	//create();
}


KSResetController::~KSResetController() {

}
  
  
TaskHandle_t KSResetController::create() {
	int coreID = xPortGetCoreID();
	UBaseType_t setupPriority = uxTaskPriorityGet(NULL);

	xTaskCreatePinnedToCore(
    	[](void* context){ static_cast<KSResetController*>(context)->tKSResetController(); },
		"tKSResetController",
		8192,
		this,
		setupPriority,
		&_htKSResetController,
		coreID
		);

	return _htKSResetController;
}
        
 


void KSResetController::tKSResetController() {
	LOGGER.println("created Task tKSResetController");
	for (;;) {
		unsigned long currentMillis = millis();
		if (currentMillis >= _resetTimeInMillis) {

			LOGGER.print("ResetController reset after ");
			LOGGER.print(currentMillis);
			LOGGER.println("ms. Restarting ESP ...");
			LOGGER.flush();
			
			if (onResetListener) {
				onResetListener();
			}
			reset();
		} else {
/*			LOGGER.print("Check Resetcontroller current ms: ");
			LOGGER.print(currentMillis);
			LOGGER.print(" Reset at: ");
			LOGGER.println(_resetTimeInMillis);
*/		}
		vTaskDelay(pdMS_TO_TICKS(1000));

	}	
}



void KSResetController::reset() {
	yield();
	vTaskDelay(pdMS_TO_TICKS(100));
	yield();
	ESP.restart();
}


#endif		// #if defined USE_KSResetController || defined KSLIBRARIES_USEALL
