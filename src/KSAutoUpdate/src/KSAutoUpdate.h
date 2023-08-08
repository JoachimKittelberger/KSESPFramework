/**
 * @file KSAutoUpdate.h
 * 
 * @brief Header for KSAutoUpdate class
 * 
 * @details handles automatic update from an update-Server
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

#ifndef _KSAUTOUPDATE_H
#define _KSAUTOUPDATE_H

//#include <ArduinoOTA.h>           // http://arduino-esp8266.readthedocs.io/en/latest/ota_updates/readme.html
#include "KSStatusData/src/KSStatusData.h"


//namespace KSSemVer {
#ifdef __cplusplus
    extern "C" {
#endif
    #include "semver/semver.h"
#ifdef __cplusplus
}
#endif
//}

#define DEFAULT_AUTO_UPDATE_STATUS_IN_MIN 15                    // default Intervall for UpdateStatus
#define DEFAULT_RETRY_IN_SEC_NO_CONNECTION 4                    // default retry Intervall für UpdateStatus if we have no connection


extern const char SW_VERSION[];
extern const char PROJECT_NAME[];
extern const char updateServerIP[];

extern const char autoUpdateStatePath[];
extern const char autoUpdateReadStatusPath[];
extern const char autoUpdateWriteStatusPath[];



class KSAutoUpdate {
    public:
        KSAutoUpdate(const char *hostname = NULL);
        ~KSAutoUpdate();

        bool lookedupOnce() { return _lookupOnce; }
        void setVersionValidation(bool validate = true) { _validateVersion = validate; }
        void setStatusDataController(KSStatusDataController* pStatusDataController) { _pStatusDataController = pStatusDataController; }
        void setUpdateStatusInterval(int intervalInMin) { _updateStatusInterval = intervalInMin; }
        void setRetryStatusInterval(int intervalInSec) { _retryStatusIntervalNoConnection = intervalInSec; }

        TaskHandle_t createConnection(EventGroupHandle_t *phEventGroupNetwork = NULL);
/*
        KSAutoUpdate& setOnStartListener(void (*callback)()) { onStartListener = callback; return *this; }
        KSAutoUpdate& setOnEndListener(void (*callback)()) { onEndListener = callback; return *this; }
        KSAutoUpdate& setOnErrorListener(void (*callback)(int error)) { onErrorListener = callback; return *this; }
        KSAutoUpdate& setOnProgressListener(void (*callback)(unsigned int progress, unsigned int total)) { onProgressListener = callback; return *this; }
*/
        KSAutoUpdate& setOnChangeDeepSleepListener(void (*callback)(bool disableDeepSleep)) { onChangeDeepSleepListener = callback; return *this; }
        KSAutoUpdate& setOnUpdateNewBinfileListener(void (*callback)(String newBinfile, String newVersion)) { onUpdateNewBinfileListener = callback; return *this; }

    private:
        void tKSAutoUpdate();
        void setupKSAutoUpdate();
        
        void checkForDeepSleepState();
        void checkForNewUpdate();
        void updateStatus();

        TaskHandle_t _htKSAutoUpdate;
        EventGroupHandle_t *_phEventGroupNetwork = NULL;


        bool _lookupOnce = false;
        bool _validateVersion = true;
        KSStatusDataController* _pStatusDataController = NULL;
        int _updateStatusInterval = DEFAULT_AUTO_UPDATE_STATUS_IN_MIN;
        int _retryStatusIntervalNoConnection = DEFAULT_RETRY_IN_SEC_NO_CONNECTION;


    protected:
/*
        void (*onStartListener)();
        void (*onEndListener)();
        void (*onErrorListener)(int error);
        void (*onProgressListener)(unsigned int progress, unsigned int total);
*/
        void (*onChangeDeepSleepListener)(bool disableDeepSleep);
        void (*onUpdateNewBinfileListener)(String newBinfile, String newVersion);
};



#endif  // #define _KSAUTOUPDATE_H