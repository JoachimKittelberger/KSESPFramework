/**
 * @file KSCriticalSection.cpp
 * 
 * @brief Implementation for KSCriticalSection class
 * 
 * @details
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


#if defined USE_KSCriticalSection || defined KSLIBRARIES_USEALL     // include File in Build only if it ist defined to use it

#include "KSCriticalSection.h"
#include "KSLogger/src/KSLogger.h"



KSCriticalSection::KSCriticalSection() {
    _hEventGroup = xEventGroupCreate();
    assert(_hEventGroup);
    LeaveCriticalSection();     // init Critcal Section as free
}


KSCriticalSection::~KSCriticalSection() {
}



void KSCriticalSection::EnterCriticalSection() {
	// Warte auf Freie Critical Section:
    if (_hEventGroup) {
		// prüfen, ob Zeichnen zur Verfügung steht, ansonsten warten, bis frei
        if ((xEventGroupGetBits(_hEventGroup) & KSCRITICALSECTION_FREE) == 0) {
            //LOGGER.println(F("[KSCriticalSection] Wating for Event KSCRITICALSECTION_FREE"));
            EventBits_t eventGroupValue;
            eventGroupValue = xEventGroupWaitBits(_hEventGroup, KSCRITICALSECTION_FREE, pdFALSE, pdTRUE, portMAX_DELAY);
//            eventGroupValue = xEventGroupWaitBits(_hEventGroup, KSCRITICALSECTION_FREE, pdFALSE, pdTRUE, pdMS_TO_TICKS(2000));
            // Check for timeout
            if ((eventGroupValue & KSCRITICALSECTION_FREE) == 0) {
                LOGGER.print(F("[KSCriticalSection] Event KSCRITICALSECTION_FREE timedout (2s) in task: "));
                LOGGER.println(String(pcTaskGetTaskName(NULL)).c_str());
            }
            //LOGGER.println(F("[KSCriticalSection] Event KSCRITICALSECTION_FREE set"));
        }

		// Enter Event
        if ((xEventGroupGetBits(_hEventGroup) & KSCRITICALSECTION_FREE) != 0) {
            xEventGroupClearBits(_hEventGroup, KSCRITICALSECTION_FREE);
            //LOGGER.println(F("[KSCriticalSection] Enter Critical Section"));
        }
    }
}




void KSCriticalSection::LeaveCriticalSection() {
	// Critical Section freigeben:
    if (_hEventGroup) {
        if ((xEventGroupGetBits(_hEventGroup) & KSCRITICALSECTION_FREE) == 0) {
            xEventGroupSetBits(_hEventGroup, KSCRITICALSECTION_FREE);
            //LOGGER.println(F("[KSCriticalSection] Release Critical Section"));
        }
    }
}

   


#endif      // #if defined USE_KSCriticalSection || defined KSLIBRARIES_USEALL