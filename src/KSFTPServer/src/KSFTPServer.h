/**
 * @file KSFTPServer.h
 * 
 * @brief Header file for KSFTPServer class
 * 
 * @details 
 * 
 * @see
 *   - https://github.com/dplasa/FTPClientServer.git
 * 
 * @author Joachim Kittelberger <jkittelberger@kibesoft.de>
 * @date 20.06.2022
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

#ifndef _KSFTPSERVER_H
#define _KSFTPSERVER_H


#include <FTPServer.h>

#include "KSFileSystem/src/KSFileSystem.h"

extern const char ftpUserName[];
extern const char ftpUserPassword[];


class KSFTPServer {
    public:
        KSFTPServer(fs::FS& filesystem = LittleFS);
        ~KSFTPServer();

        TaskHandle_t createConnection(EventGroupHandle_t *phEventGroupNetwork = NULL);
        void setCredentials(const char* pUsername, const char* pPassword) { _pUsername = const_cast<char*>(pUsername); _pPassword = const_cast<char*>(pPassword); }

    private:
        void tKSFTPServer();

        FTPServer _ftpSrv;

        TaskHandle_t _htKSFTPServer;
        EventGroupHandle_t *_phEventGroupNetwork = NULL;

        char* _pUsername = const_cast<char*>(ftpUserName);
        char* _pPassword = const_cast<char*>(ftpUserPassword);
};



#endif  // #define _KSFTPSERVER_H