/**
 * @file KSStratoMailClient.cpp
 * 
 * @brief implementation of KSStratoMailClient class
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

#if defined USE_KSStratoMailClient || defined KSLIBRARIES_USEALL     // include File in Build only if it ist defined to use it

#include "KSStratoMailClient.h"



// Callback function to get the Email sending status
void smtpCallback(SMTP_Status status)
{
  // Print the current status
  Serial.println(status.info());

  // Print the sending result
  if (status.success())
  {
    Serial.println("----------------");
    ESP_MAIL_PRINTF("Message sent success: %d\n", status.completedCount());
    ESP_MAIL_PRINTF("Message sent failled: %d\n", status.failedCount());
    Serial.println("----------------\n");

/*
    struct tm dt;

    for (size_t i = 0; i < _smtp.sendingResult.size(); i++)
//    for (size_t i = 0; i < KSStratoMailClient::_smtp.sendingResult.size(); i++)
    {
      // Get the result item
      SMTP_Result result = _smtp.sendingResult.getItem(i);
//      SMTP_Result result = KSStratoMailClient::_smtp.sendingResult.getItem(i);
      time_t ts = (time_t)result.timestamp;
      KSRTC::time_t2TimeInfo(&ts, &dt);

      ESP_MAIL_PRINTF("Message No: %d\n", i + 1);
      ESP_MAIL_PRINTF("Status: %s\n", result.completed ? "success" : "failed");
      ESP_MAIL_PRINTF("Date/Time: %d/%d/%d %d:%d:%d\n", dt.tm_year + 1900, dt.tm_mon + 1, dt.tm_mday, dt.tm_hour, dt.tm_min, dt.tm_sec);
      ESP_MAIL_PRINTF("Recipient: %s\n", result.recipients.c_str());
      ESP_MAIL_PRINTF("Subject: %s\n", result.subject.c_str());
    }
    Serial.println("----------------\n");

    //You need to clear sending result as the memory usage will grow up.
    _smtp.sendingResult.clear();
//    KSStratoMailClient::_smtp.sendingResult.clear();
*/
  }
}


void KSStratoMailMessage::addAttachment(const char* filename) {

    // The attachment data item
    SMTP_Attachment att;

    // Set the inline image info e.g. file name, MIME type, file path, file storage type,
    // transfer encoding and content encoding
    att.descr.filename = &filename[1];	// TODO: hier sollte der String ohne vorangestellten "/"" stehen
    att.descr.mime = F("text/plain");
    att.file.path = filename;

    // The file storage type e.g. 
    // esp_mail_file_storage_type_none, 
    // esp_mail_file_storage_type_flash, and 
    // esp_mail_file_storage_type_sd 
    att.file.storage_type = esp_mail_file_storage_type_flash;

    // Need to be base64 transfer encoding for inline image
    att.descr.transfer_encoding = Content_Transfer_Encoding::enc_base64;

    // Add attachment to the message
    _message.addAttachment(att);

}


#endif    // #if defined USE_KSStratoMailClient || defined KSLIBRARIES_USEALL