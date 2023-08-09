/**
 * @file KSStratoMailClient.h
 * 
 * @brief Definition of KSStratoMailClient class
 * 
 * @details
 * 
 * @see
 *   - https://github.com/mobizt/ESP-Mail-Client.git
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

#pragma once

#include <ESP_Mail_Client.h>

extern const char smtpHost[];
extern const uint16_t SMTP_PORT;


// Callback function to get the Email sending status
void smtpCallback(SMTP_Status status);



class KSStratoMailMessage {
    public:
        KSStratoMailMessage() {
            init();
        }
        KSStratoMailMessage(const char* sender_name, const char* sender_email) {
            init();
            setSender(sender_name, sender_email);
        }
        ~KSStratoMailMessage() {}

        void setSender(const char* name, const char* email) {
            _message.sender.name = name;
            _message.sender.email = email;
        }

        void setRecipient(const char* name, const char* email) {
            _message.addRecipient(name, email);
        }
        void setSubject(const char* subject) {
            _message.subject = subject;
        }
        void setTextContent(const char* content) {
            _message.text.content = content;
        }
        void setHtmlContent(const char* content) {
            //String htmlMsg = "<div style=\"color:#2f4468;\"><h1>Hello World!</h1><p>- Sent from ESP board</p></div>";
            _message.html.content = content;
        }
        void setPriorityHigh() {
            _message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_high;
        }

        void addAttachment(const char* filename);


        SMTP_Message* getSMTPMessagePtr() { return &_message; }
    
    protected:
        void init() {
            _message.text.charSet = F("utf-8");
            _message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
            _message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_normal;
            // default initialisation
//            _message.sender.name = AUTHOR_NAME;
//            _message.sender.email = AUTHOR_EMAIL;
        }


    private:
        SMTP_Message _message;
};



class KSStratoMailClient {
    public:
        KSStratoMailClient(const char* account_email, const char* account_password, const char* pHostName = smtpHost, uint16_t port = SMTP_PORT) {
            _smtp.debug(0);             // debug(1) = basic debug output, debug(0) = no debug output
            _session.server.host_name = pHostName;
            _session.server.port = port;

            _session.login.email = account_email;
            _session.login.password = account_password;
        }
        ~KSStratoMailClient() {}

        bool sendMail(KSStratoMailMessage* pMessage);
        //void smtpCallback(SMTP_Status status);

    private:
        SMTPSession _smtp;
        ESP_Mail_Session _session;
};


