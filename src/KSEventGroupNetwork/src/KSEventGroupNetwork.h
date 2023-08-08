/**
 * @file KSEventGroupNetwork.h
 * 
 * @brief Header for KSEventGroupNetwork
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


#ifndef _EVENTGROUPNETWORK_H
#define _EVENTGROUPNETWORK_H




// definition of bits for EventGroupNetwork
#define EG_NETWORK_INITIALIZED (1UL << 0UL)        // zero shift for bit0  // WiFi is initialized
#define EG_NETWORK_CONNECTED (1UL << 1UL)          // 1 shift for bit1     // WiFi is connected


#define EG_NETWORK_MQTT_CONNECTING (1UL << 2UL)    // 2 shift for bit2     // MQTT connection active
#define EG_NETWORK_MQTT_CONNECTED (1UL << 3UL)     // 3 shift for bit3     // MQTT is connected

#define EG_NETWORK_NTP_SYNCING  (1UL << 4UL)       // 4 shift for bit4     // NTP sync active
#define EG_NETWORK_NTP_SYNCED  (1UL << 5UL)        // 5 shift for bit5     // NTP is synced

#define EG_NETWORK_FS_AVAILABLE (1UL << 6UL)       // 6 shift for bit6     // FileSystem is available
#define EG_NETWORK_SAVE_ACTIVE (1UL << 7UL)        // 7 shift for bit7     // Save of data is active

#define EG_NETWORK_JET32_CONNECTING (1UL << 8UL)       // 8 shift for bit8     // Jet32 connecting active
#define EG_NETWORK_JET32_CONNECTED (1UL << 9UL)        // 9 shift for bit9     // Jet32 connected


/*
    // set Events
    if (_phEventGroupNetwork && (*_phEventGroupNetwork != NULL)) {
        if ((xEventGroupGetBits(*_phEventGroupNetwork) & EG_DISPLAY_INITIALIZED) == 0) {
            xEventGroupSetBits(*_phEventGroupNetwork, EG_DISPLAY_INITIALIZED);
//            Serial.println(F("[display] Set Event EG_DISPLAY_INITIALIZED"));
        }
    }

    
    // Wait for Evemts
    if (_hEventGroup) {
		// prüfen, ob Zeichnen zur Verfügung steht, ansonsten warten, bis frei
        if ((xEventGroupGetBits(_hEventGroup) & KSCRITICALSECTION_FREE) == 0) {
            //Serial.println(F("[KSCriticalSection] Wating for Event KSCRITICALSECTION_FREE"));
            EventBits_t eventGroupValue;
            eventGroupValue = xEventGroupWaitBits(_hEventGroup, KSCRITICALSECTION_FREE, pdFALSE, pdTRUE, portMAX_DELAY);
            //Serial.println(F("[KSCriticalSection] Event KSCRITICALSECTION_FREE set"));
        }
    }

    // Reset Events
    if ((xEventGroupGetBits(_hEventGroup) & KSCRITICALSECTION_FREE) == KSCRITICALSECTION_FREE) {
        xEventGroupClearBits(_hEventGroup, KSCRITICALSECTION_FREE);
        //Serial.println(F("[KSCriticalSection] Enter Critical Section"));
    }
*/

#endif  // #define _EVENTGROUPNETWORK_H