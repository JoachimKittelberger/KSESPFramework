/**
 * @file KSDisplay.cpp
 * 
 * @brief Implementation for KSDisplay class
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

#if defined USE_KSDisplay || defined KSLIBRARIES_USEALL     // include File in Build only if it ist defined to use it


#include "KSDisplay.h"
#include "KSUtilities/src/KSUtilities.h"
//#include "projectinfo.h"

#include "KSEventGroupDisplay/src/KSEventGroupDisplay.h"
#include "KSEventGroupNetwork/src/KSEventGroupNetwork.h"




#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 
static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
	B00000001, B11000000,
	B00000001, B11000000,
	B00000011, B11100000,
	B11110011, B11100000,
	B11111110, B11111000,
	B01111110, B11111111,
	B00110011, B10011111,
	B00011111, B11111100,
	B00001101, B01110000,
	B00011011, B10100000,
	B00111111, B11100000,
	B00111111, B11110000,
	B01111100, B11110000,
	B01110000, B01110000,
	B00000000, B00110000 };

#define LOGOKIBESOFT_HEIGHT 128
#define LOGOKIBESOFT_WIDTH 15
const unsigned char logoKibeSoft_bmp[] PROGMEM = {
	// 'Firmenname, 128x15px
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x1e, 0x07, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x1e, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x1e, 0x3e, 0x01, 0x83, 0xff, 0x81, 0xff, 0xe0, 0xc0, 0x1c, 0x07, 0xff, 0x03, 0xff, 0x9f, 0xff,
	0x1e, 0x78, 0x01, 0x83, 0xff, 0xc1, 0xff, 0xe0, 0xc0, 0x10, 0x1f, 0xff, 0x83, 0xff, 0x9f, 0xff,
	0x1e, 0xf0, 0x01, 0x83, 0x80, 0xe1, 0xc0, 0x00, 0xc0, 0x00, 0x1c, 0x03, 0x83, 0x00, 0x00, 0xe0,
	0x1f, 0xe0, 0x01, 0x83, 0x80, 0xe1, 0xc0, 0x00, 0xff, 0xf8, 0x1c, 0x03, 0x83, 0x00, 0x00, 0xe0,
	0x1f, 0xc0, 0x01, 0x83, 0x80, 0xc1, 0xc0, 0x00, 0xff, 0xfc, 0x1c, 0x03, 0x83, 0x00, 0x00, 0xe0,
	0x1f, 0xe0, 0x01, 0x83, 0xff, 0x81, 0xfe, 0x00, 0x7f, 0xfc, 0x1c, 0x03, 0x83, 0xf8, 0x00, 0xe0,
	0x1e, 0xf8, 0x01, 0x83, 0xff, 0xc1, 0xfe, 0x00, 0x00, 0x1c, 0x1c, 0x03, 0x83, 0xf8, 0x00, 0xe0,
	0x1e, 0x7c, 0x01, 0x83, 0x80, 0xe1, 0xc0, 0x00, 0xc0, 0x1c, 0x1c, 0x03, 0x83, 0x00, 0x00, 0xe0,
	0x1e, 0x1f, 0x01, 0x83, 0x80, 0xe1, 0xc0, 0x01, 0xc0, 0x1c, 0x1c, 0x03, 0x83, 0x00, 0x00, 0xe0,
	0x1e, 0x0f, 0x81, 0x83, 0xff, 0xe1, 0xff, 0xe1, 0xff, 0xfc, 0x1f, 0xff, 0x83, 0x00, 0x00, 0xe0,
	0x1e, 0x07, 0xc1, 0x83, 0xff, 0xc1, 0xff, 0xe0, 0xff, 0xf8, 0x0f, 0xff, 0x83, 0x00, 0x00, 0xe0,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};




KSDisplay::KSDisplay() : _display(KS_OLED_SDA, KS_OLED_SCL) {
	_SDA = KS_OLED_SDA;
	_SCL = KS_OLED_SCL;
	onScreenSaver = NULL;
	initiScreenSaver();
}


KSDisplay::KSDisplay(uint8_t SDA, uint8_t SCL, KSCriticalSection* pcsI2C) : _display(SDA, SCL) {
	_SDA = SDA;
	_SCL = SCL;
	_pcsI2C = pcsI2C;
	onScreenSaver = NULL;
	initiScreenSaver();
}



KSDisplay::~KSDisplay() {
}



TaskHandle_t KSDisplay::createConnection(EventGroupHandle_t *phEventGroupNetwork, EventGroupHandle_t *phEventGroupDisplay) {

	_phEventGroupNetwork = phEventGroupNetwork;
	_phEventGroupDisplay = phEventGroupDisplay;

	int coreID = xPortGetCoreID();
	//Serial.print(F("CoreID: "));
	//Serial.println(coreID);
	
	UBaseType_t setupPriority = uxTaskPriorityGet(NULL);
	//Serial.print(F("setup: priority = "));
	//Serial.println(setupPriority);

	xTaskCreatePinnedToCore(
        [](void* context){ static_cast<KSDisplay*>(context)->tKSDisplay(); },
		"tKSDisplay",
		8192,
		this,
		setupPriority,
		&_htKSDisplay,
		coreID
		);

	return _htKSDisplay;
}



void KSDisplay::tKSDisplay()
{
	init();
	showSplashscreen();

	_bIsInit = true; 
	// ab hier Event an mqtt, dass Wifi initialisiert ist. Sonst stürzt WiFi ab, wenn mqtt zu schnell darauf zugreift
	if (_phEventGroupDisplay && (*_phEventGroupDisplay != NULL)) {
		if ((xEventGroupGetBits(*_phEventGroupDisplay) & EG_DISPLAY_INITIALIZED) == 0) {
				xEventGroupSetBits(*_phEventGroupDisplay, EG_DISPLAY_INITIALIZED);
		}
	}

	// main loop display
	for (;;) {
		// Screensaver berechnen und dan die Updates übergehn
		// BlankScreen nach Screensaver einführen
		// evtl für aussen Anfrage für Screensaver einführen

		// check deactivation of screen
		if (!isInDeepSleep() && (millis() > _millisToDeactivateScreen)) {
			sleepDisplay();
		}

		UpdateStatusBar();


		vTaskDelay(pdMS_TO_TICKS(200));
	}
}



void KSDisplay::init() {
	pinMode(_SDA, INPUT_PULLUP);
	pinMode(_SCL, INPUT_PULLUP);

		
	if (_pcsI2C) _pcsI2C->EnterCriticalSection();
	Wire.begin(_SDA, _SCL, 400000);
	
	// by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
	_display.begin(SH1106_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
	if (_pcsI2C) _pcsI2C->LeaveCriticalSection();
}




bool KSDisplay::isInit() {
	// Wenn EventGroup definiert, dann diese Info verwenden
	if (_phEventGroupDisplay && (*_phEventGroupDisplay != NULL)) {
		if ((xEventGroupGetBits(*_phEventGroupDisplay) & EG_DISPLAY_INITIALIZED) == EG_DISPLAY_INITIALIZED) {
			return true;
		}
	}
	return _bIsInit;
}


void KSDisplay::waitForInit() {
	// Warte auf Initialisierung:
	if (_phEventGroupDisplay && (*_phEventGroupDisplay != NULL)) {
		if ((xEventGroupGetBits(*_phEventGroupDisplay) & EG_DISPLAY_INITIALIZED) == 0) {
//            Serial.println(F("[display] Wating for Event EG_DISPLAY_INITIALIZED"));
			EventBits_t eventGroupValue;
			eventGroupValue = xEventGroupWaitBits(*_phEventGroupDisplay, EG_DISPLAY_INITIALIZED, pdFALSE, pdTRUE, portMAX_DELAY);
//            Serial.println(F("[display] Event EG_DISPLAY_INITIALIZED set"));
			}
	} else {
		while (!_bIsInit) {
			vTaskDelay(pdMS_TO_TICKS(100));
		}
	}
}



void KSDisplay::showSplashscreen() {
	showLogo(2000);
	showAdressInfo(2000);
}



// set value for next Screensaver activation in ms
void KSDisplay::activateForDurationInMS(unsigned long duration) {
	if (duration == ULONG_MAX) {
		_millisToDeactivateScreen = duration;
	} else {
		_millisToDeactivateScreen = millis() + duration;
	}
	
	// if we are in deepsleep wakeup display
	if (isInDeepSleep())
		wakeupDisplay();
}




void KSDisplay::sleepDisplay() {
	if (_pcsI2C) _pcsI2C->EnterCriticalSection();
	_display.sh1106_command(SH1106_DISPLAYOFF);
	if (_pcsI2C) _pcsI2C->LeaveCriticalSection();

	_bDisplayInDeepSleep = true;
	if (onScreenSaver)
		onScreenSaver(_bDisplayInDeepSleep);
}


void KSDisplay::wakeupDisplay() {
	if (_pcsI2C) _pcsI2C->EnterCriticalSection();
	_display.sh1106_command(SH1106_DISPLAYON);
	if (_pcsI2C) _pcsI2C->LeaveCriticalSection();
	_bDisplayInDeepSleep = false;
	if (onScreenSaver)
		onScreenSaver(_bDisplayInDeepSleep);
}







void KSDisplay::initiScreenSaver(){
	// initialize screensaver
	for (uint8_t f=0; f< NUMFLAKES; f++) {
		_icons[f][XPOS] = random(_display.width());
		_icons[f][YPOS] = 0;
		_icons[f][DELTAY] = random(5) + 1;
	}
}




void KSDisplay::showScreenSaver(unsigned long duration, boolean forever)
{
	// draw a bitmap icon and 'animate' movement
	//_display.clearDisplay();
	clearDisplay();
	
	const uint8_t *bitmap = logo16_glcd_bmp;
	uint8_t w = LOGO16_GLCD_HEIGHT;
	uint8_t h = LOGO16_GLCD_WIDTH;

	//uint8_t icons[NUMFLAKES][3];
	
	// initialize
/*    for (uint8_t f=0; f< NUMFLAKES; f++) {
				_icons[f][XPOS] = random(_display.width());
				_icons[f][YPOS] = 0;
				_icons[f][DELTAY] = random(5) + 1;
				
		//  Serial.print("x: ");
		//  Serial.print(icons[f][XPOS], DEC);
		//  Serial.print(" y: ");
		//  Serial.print(icons[f][YPOS], DEC);
		//  Serial.print(" dy: ");
		//  Serial.println(icons[f][DELTAY], DEC);
		}
*/
	unsigned long startMillis = millis();
	unsigned long currentMillis;
	while (true) {
		currentMillis = millis();
		if (!forever && (currentMillis - startMillis >= duration)) {
			break;
		}

		// draw each icon
		if (_pcsI2C) _pcsI2C->EnterCriticalSection();
		for (uint8_t f=0; f< NUMFLAKES; f++) {
			_display.drawBitmap(_icons[f][XPOS], _icons[f][YPOS], bitmap, w, h, WHITE);
		}
		_display.display();
		if (_pcsI2C) _pcsI2C->LeaveCriticalSection();

		vTaskDelay(pdMS_TO_TICKS(200));

			// then erase it + move it
		if (_pcsI2C) _pcsI2C->EnterCriticalSection();
		for (uint8_t f=0; f< NUMFLAKES; f++) {
			_display.drawBitmap(_icons[f][XPOS], _icons[f][YPOS],  bitmap, w, h, BLACK);
			// move it
			_icons[f][YPOS] += _icons[f][DELTAY];
			// if its gone, reinit
			if (_icons[f][YPOS] > _display.height()) {
				_icons[f][XPOS] = random(_display.width());
				_icons[f][YPOS] = 0;
				_icons[f][DELTAY] = random(5) + 1;
			}
		}
		if (_pcsI2C) _pcsI2C->LeaveCriticalSection();
	}

	//_display.clearDisplay();
	clearDisplay();

}


void KSDisplay::showLogo(unsigned long delayMS) {
	if (_pcsI2C) _pcsI2C->EnterCriticalSection();

		// show the KibeSoft logo as splash screen
	_display.clearDisplay();
	_display.drawBitmap(0, 10, logoKibeSoft_bmp, LOGOKIBESOFT_HEIGHT, LOGOKIBESOFT_WIDTH, 1);
	_display.setTextSize(1);				 // Normal 1:1 pixel scale
	_display.setTextColor(WHITE); // Draw white text
	_display.setCursor(4, 30);
	_display.print(F("Joachim Kittelberger"));
	_display.display();

	if (_pcsI2C) _pcsI2C->LeaveCriticalSection();
	vTaskDelay(pdMS_TO_TICKS(delayMS));
}


void KSDisplay::showAdressInfo(unsigned long delayMS) {
	if (_pcsI2C) _pcsI2C->EnterCriticalSection();

	// Clear the buffer
	_display.clearDisplay();

	_display.setTextSize(1);				 // Normal 1:1 pixel scale
	_display.setTextColor(WHITE); // Draw white text
	_display.cp437(true);				 // Use full 256 char 'Code Page 437' font

	_display.setCursor(0, 0); // Start at top-left corner
	_display.print(F("Joachim Kittelberger"));
	_display.setCursor(0, 10);
	_display.print(F("Kirchhofaecker 2"));
	_display.setCursor(0, 20);
	_display.print(F("71737 Kirchberg"));
	_display.setCursor(0, 30);
	_display.print(F("www.KibeSoft.de"));
	_display.setCursor(0, 45);

	// display the current Date and SW-Version
	char currentdate[12] = { 0 };
	formatDATE2DE(__DATE__, currentdate, sizeof(currentdate));
	_display.print(currentdate);
	_display.print(F("  v"));
	_display.print(F(SW_VERSION));

	_display.setCursor(0, 55);
	_display.print(PROJECT_NAME);
	_display.display();
	if (_pcsI2C) _pcsI2C->LeaveCriticalSection();

	vTaskDelay(pdMS_TO_TICKS(delayMS));
}




#define BLINKDURATION_MS 300

void KSDisplay::UpdateStatusBar() {
	if (_pcsI2C) _pcsI2C->EnterCriticalSection();
	// TODO: show Statusbar, Update im Sekunden-Rythmus?
	
	
	// lösche bisherige StatusBar
	_display.fillRect(0, 0, _display.width(), 8, BLACK);

	// print RTC if available
	if (_pRTC) {
		
		if (_pcsI2C) _pcsI2C->LeaveCriticalSection();
		String strTime;
		strTime = String(_pRTC->getLocalTimeFormated());
		strTime.setCharAt(8, '\0');

		if (_pcsI2C) _pcsI2C->EnterCriticalSection();
		_display.setTextSize(1);
		_display.setCursor(0, 0);
		_display.print(strTime.c_str());
	}
	if (_pcsI2C) _pcsI2C->LeaveCriticalSection();


	// evtl auch ausserhalb cvon update schon ermitteln und einzelne Funktionen aufrufen

	// calculate Blinking
	static bool bBlinkOff = false;
	static unsigned long startMillis = 0;
	unsigned long currentMillis = millis();
	if (currentMillis - startMillis >= BLINKDURATION_MS) {
		startMillis = currentMillis;
		bBlinkOff = !bBlinkOff;
	}

	char szStatus[7] = "      ";
	EventBits_t eventGroupNetworkBits = 0;
	if (_phEventGroupNetwork && (*_phEventGroupNetwork != NULL)) {
		eventGroupNetworkBits =	xEventGroupGetBits(*_phEventGroupNetwork);
	}
	if (eventGroupNetworkBits & EG_NETWORK_INITIALIZED) {
		if (!bBlinkOff) szStatus[0] = 'w';
	}
	if (eventGroupNetworkBits & EG_NETWORK_CONNECTED) {
		szStatus[0] = 'W';
	}
	if (eventGroupNetworkBits & EG_NETWORK_MQTT_CONNECTING) {
		if (!bBlinkOff) szStatus[1] = 'm';
	}
	if (eventGroupNetworkBits & EG_NETWORK_MQTT_CONNECTED) {
		szStatus[1] = 'M';
	}
	if (eventGroupNetworkBits & EG_NETWORK_NTP_SYNCING) {
		if (!bBlinkOff) szStatus[2] = 't';
	}
	if (eventGroupNetworkBits & EG_NETWORK_NTP_SYNCED) {
		szStatus[2] = 'T';
	}
	if (eventGroupNetworkBits & EG_NETWORK_FS_AVAILABLE) {
		szStatus[3] = 'F';
	}
	if (eventGroupNetworkBits & EG_NETWORK_SAVE_ACTIVE) {
		if (!bBlinkOff) szStatus[4] = 'S';
	}
	if (eventGroupNetworkBits & EG_NETWORK_JET32_CONNECTING) {
		if (!bBlinkOff) szStatus[5] = 'j';
	}
	if (eventGroupNetworkBits & EG_NETWORK_JET32_CONNECTED) {
		szStatus[5] = 'J';
	}
	szStatus[6] = '\0';


	if (_pcsI2C) _pcsI2C->EnterCriticalSection();
	_display.setTextSize(1);		// Font is 5x7 with 1 space between
	_display.setCursor(_display.width()-1 - (6*5 + 5*1), 0);		// 6 chars and 5 spaces
	_display.print(szStatus);



//	_display.drawRect(_display.width()/2 - 1, 0, _display.width()/2, 8, WHITE);

	_display.display();
	if (_pcsI2C) _pcsI2C->LeaveCriticalSection();
}









// dimensions of Scrollbar
#define SB_TOP 0
#define SB_WIDTH 8
#define SB_HIGHT 64
#define SB_ROUNDED 3
#define SB_INNERWIDTH 4
#define SB_DEFAULTHEIGHT 10
#define SB_MAXENTRY_PERPAGE 8
#define SB_FRAMETHICKNESS_TOINNER ((SB_WIDTH - SB_INNERWIDTH) / 2)
#define SB_INNERHEIGHT (SB_HIGHT - (SB_FRAMETHICKNESS_TOINNER * 2))


void KSDisplay::DrawScrollbar(int percent, int maxEntries) {
	if (percent > 100)
		percent = 100;
	if (percent < 0)
		percent = 0;

	// calculate tht Height of the scrollbar
	int sbHeight = SB_INNERHEIGHT;
	if (maxEntries > 0)
	{
		sbHeight = SB_INNERHEIGHT * SB_MAXENTRY_PERPAGE / maxEntries;
	}

	// bring it in range
	if (sbHeight > SB_INNERHEIGHT)
		sbHeight = SB_INNERHEIGHT;
	if (sbHeight < SB_DEFAULTHEIGHT)
		sbHeight = SB_DEFAULTHEIGHT;
	//Serial.printf("sbHeight: %d\n", sbHeight);

	// calculate the Position of the scrollbar
	int sbTopPosition = SB_TOP + SB_FRAMETHICKNESS_TOINNER;
	int sbBottomPosition = SB_TOP + SB_HIGHT - SB_FRAMETHICKNESS_TOINNER;
	//Serial.printf("sbTopPosition: %d\n", sbTopPosition);
	//Serial.printf("sbBottomPosition: %d\n", sbBottomPosition);

	int sbPixelRange = (sbBottomPosition - sbTopPosition) - sbHeight;
	if (sbPixelRange <= 0)
		sbPixelRange = 1;
	int sbStartPosition = sbTopPosition;
	if (percent > 0)
	{
		sbStartPosition = sbTopPosition + (sbPixelRange * 100 / percent);
	}
	// bring it in range
	if (sbStartPosition < sbTopPosition)
		sbStartPosition = sbTopPosition;
	if (sbStartPosition + sbHeight > sbBottomPosition)
		sbStartPosition = sbBottomPosition - sbHeight;
	//Serial.printf("sbStartPosition: %d\n", sbStartPosition);

	// draw outer rectangle on the right side
	_display.drawRoundRect(_display.width() - SB_WIDTH, SB_TOP, SB_WIDTH, SB_HIGHT, SB_ROUNDED, WHITE);
	// draw scrollbar inner rectangle
	_display.fillRoundRect(_display.width() - SB_WIDTH + SB_FRAMETHICKNESS_TOINNER, sbStartPosition,
							SB_INNERWIDTH, sbHeight, SB_ROUNDED, INVERSE);

	// scrollbar malen
	//	_display.drawRoundRect(120, 0, 8, 64, 3, SSD1306_WHITE);
	//	_display.fillRoundRect(122, 2, 4, 60, 3, SSD1306_INVERSE);
}





void KSDisplay::ShowCurrentPPMPage(int ppm, float temperature) {

	if (_pcsI2C) _pcsI2C->EnterCriticalSection();

	//_display.clearDisplay();
	// lösche bisheriger Anzeigebereich
	_display.fillRect(0, 8, _display.width(), _display.height() - 8, BLACK);

	// Fontsize 1 = 5?x7?
	// Fontsize 2 = 10x16? und 2 Zwischenraum -> Ofset 8 für 10 Zeichen in einer Reihe rechtsbündig

	// display CO2
	_display.setTextSize(1);
	_display.setCursor(0, 20);
	_display.print(F("CO2: "));
	_display.setTextSize(2);
	_display.setCursor(8 + 2*12, 20);
	if (ppm < 0) {
		_display.print(F("----"));
	} else {
		char szBuffer[10];
		snprintf(szBuffer, sizeof(szBuffer), "%4d", ppm);
		_display.print(szBuffer);
	}
	_display.print(F(" ppm"));

	// display termperature
	_display.setTextSize(1);
	_display.setCursor(0, 50);
	_display.print(F("Temp: "));
	_display.setTextSize(2);
	_display.setCursor(8 + 3*12, 50);
	if (temperature < 0) {
		_display.print(F("----"));
	} else {
		char szBuffer[10];
		snprintf(szBuffer, sizeof(szBuffer), "%4.1f", temperature);
//		dtostrf(temperature, 4, 1, szBuffer);
		_display.print(szBuffer);
	}
	_display.print(F(" "));
	_display.cp437(true);
	_display.write(167);
//	_display.write(247);
	_display.print(F("C"));

	_display.display();
	if (_pcsI2C) _pcsI2C->LeaveCriticalSection();
}




// Line is from 0..7
void KSDisplay::printTextInLine(int line, const char* pText) {

	if (_pcsI2C) _pcsI2C->EnterCriticalSection();

	// show maximum 21 char per line
	char szBuffer[22] = { 0 };
	strncpy(szBuffer, pText, sizeof(szBuffer) - 1);
	szBuffer[sizeof(szBuffer) - 1] = '\0';

	// lösche bisheriger Anzeigebereich
	int16_t xPosTop = 8 * line;
	_display.fillRect(0, xPosTop, _display.width(), 8, BLACK);

	// Fontsize 1 = 5?x7?
	// Fontsize 2 = 10x16? und 2 Zwischenraum -> Ofset 8 für 10 Zeichen in einer Reihe rechtsbündig

	// display text
	_display.setTextSize(1);
	_display.setCursor(0, xPosTop);
	_display.print(szBuffer);

	_display.display();
	if (_pcsI2C) _pcsI2C->LeaveCriticalSection();
}




void KSDisplay::testDrawAllChar(int time2waitms) {
	if (_pcsI2C) _pcsI2C->EnterCriticalSection();
	_display.clearDisplay();

	// draw the first ~12 characters in the font
	_display.setTextSize(1);
	_display.setTextColor(WHITE);

	_display.setCursor(0,0);
	for (uint8_t i=0; i < 168; i++) {
		if (i == '\n') continue;
		_display.write(i);
		if ((i > 0) && (i % 21 == 0))
		_display.println();
	}    
	_display.display();
	if (_pcsI2C) _pcsI2C->LeaveCriticalSection();

	delay(time2waitms);


	if (_pcsI2C) _pcsI2C->EnterCriticalSection();
	_display.clearDisplay();
	_display.setCursor(0,0);
	for (uint8_t i=169; i < 256; i++) {
		if (i == '\n') continue;
		_display.write(i);
		if ((i > 0) && (i % 21 == 0))
		_display.println();
	}    
	_display.display();
	if (_pcsI2C) _pcsI2C->LeaveCriticalSection();

		delay(time2waitms);

}



/*

	// Show image buffer on the display hardware.
	// Since the buffer is intialized with an Adafruit splashscreen
	// internally, this will display the splashscreen.
	display.display();
	delay(2000);

	// Clear the buffer.
	display.clearDisplay();

	// draw a single pixel
	display.drawPixel(10, 10, WHITE);
	// Show the display buffer on the hardware.
	// NOTE: You _must_ call display after making any drawing commands
	// to make them visible on the display hardware!
	display.display();
	delay(2000);
	display.clearDisplay();

	// draw many lines
	testdrawline();
	display.display();
	delay(2000);
	display.clearDisplay();

	// draw rectangles
	testdrawrect();
	display.display();
	delay(2000);
	display.clearDisplay();

	// draw multiple rectangles
	testfillrect();
	display.display();
	delay(2000);
	display.clearDisplay();

	// draw mulitple circles
	testdrawcircle();
	display.display();
	delay(2000);
	display.clearDisplay();

	// draw a white circle, 10 pixel radius
	display.fillCircle(display.width()/2, display.height()/2, 10, WHITE);
	display.display();
	delay(2000);
	display.clearDisplay();

	testdrawroundrect();
	delay(2000);
	display.clearDisplay();

	testfillroundrect();
	delay(2000);
	display.clearDisplay();

	testdrawtriangle();
	delay(2000);
	display.clearDisplay();
	 
	testfilltriangle();
	delay(2000);
	display.clearDisplay();

	// draw the first ~12 characters in the font
	testdrawchar();
	display.display();
	delay(2000);
	display.clearDisplay();

	// draw scrolling text
	testscrolltext();
	delay(2000);
	display.clearDisplay();

	// text display tests
	display.setTextSize(1);
	display.setTextColor(WHITE);
	display.setCursor(0,0);
	display.println("Hello, world!");
	display.setTextColor(BLACK, WHITE); // 'inverted' text
	display.println(3.141592);
	display.setTextSize(2);
	display.setTextColor(WHITE);
	display.print("0x"); display.println(0xDEADBEEF, HEX);
	display.display();
	delay(20000);

	// miniature bitmap display
	display.clearDisplay();
	display.drawBitmap(30, 16,  logo16_glcd_bmp, 16, 16, 1);
	display.display();

	// invert the display
	display.invertDisplay(true);
	delay(1000); 
	display.invertDisplay(false);
	delay(1000); 

	// draw a bitmap icon and 'animate' movement
	testdrawbitmap(logo16_glcd_bmp, LOGO16_GLCD_HEIGHT, LOGO16_GLCD_WIDTH);


*/



/*

void testdrawchar(void) {
	display.setTextSize(1);
	display.setTextColor(WHITE);
	display.setCursor(0,0);

	for (uint8_t i=0; i < 168; i++) {
		if (i == '\n') continue;
		display.write(i);
		if ((i > 0) && (i % 21 == 0))
			display.println();
	}    
	display.display();
}

void testdrawcircle(void) {
	for (int16_t i=0; i<display.height(); i+=2) {
		display.drawCircle(display.width()/2, display.height()/2, i, WHITE);
		display.display();
	}
}

void testfillrect(void) {
	uint8_t color = 1;
	for (int16_t i=0; i<display.height()/2; i+=3) {
		// alternate colors
		display.fillRect(i, i, display.width()-i*2, display.height()-i*2, color%2);
		display.display();
		color++;
	}
}

void testdrawtriangle(void) {
	for (int16_t i=0; i<min(display.width(),display.height())/2; i+=5) {
		display.drawTriangle(display.width()/2, display.height()/2-i,
										 display.width()/2-i, display.height()/2+i,
										 display.width()/2+i, display.height()/2+i, WHITE);
		display.display();
	}
}

void testfilltriangle(void) {
	uint8_t color = WHITE;
	for (int16_t i=min(display.width(),display.height())/2; i>0; i-=5) {
		display.fillTriangle(display.width()/2, display.height()/2-i,
										 display.width()/2-i, display.height()/2+i,
										 display.width()/2+i, display.height()/2+i, WHITE);
		if (color == WHITE) color = BLACK;
		else color = WHITE;
		display.display();
	}
}

void testdrawroundrect(void) {
	for (int16_t i=0; i<display.height()/2-2; i+=2) {
		display.drawRoundRect(i, i, display.width()-2*i, display.height()-2*i, display.height()/4, WHITE);
		display.display();
	}
}

void testfillroundrect(void) {
	uint8_t color = WHITE;
	for (int16_t i=0; i<display.height()/2-2; i+=2) {
		display.fillRoundRect(i, i, display.width()-2*i, display.height()-2*i, display.height()/4, color);
		if (color == WHITE) color = BLACK;
		else color = WHITE;
		display.display();
	}
}
	 
void testdrawrect(void) {
	for (int16_t i=0; i<display.height()/2; i+=2) {
		display.drawRect(i, i, display.width()-2*i, display.height()-2*i, WHITE);
		display.display();
	}
}

void testdrawline() {  
	for (int16_t i=0; i<display.width(); i+=4) {
		display.drawLine(0, 0, i, display.height()-1, WHITE);
		display.display();
	}
	for (int16_t i=0; i<display.height(); i+=4) {
		display.drawLine(0, 0, display.width()-1, i, WHITE);
		display.display();
	}
	delay(250);
	
	display.clearDisplay();
	for (int16_t i=0; i<display.width(); i+=4) {
		display.drawLine(0, display.height()-1, i, 0, WHITE);
		display.display();
	}
	for (int16_t i=display.height()-1; i>=0; i-=4) {
		display.drawLine(0, display.height()-1, display.width()-1, i, WHITE);
		display.display();
	}
	delay(250);
	
	display.clearDisplay();
	for (int16_t i=display.width()-1; i>=0; i-=4) {
		display.drawLine(display.width()-1, display.height()-1, i, 0, WHITE);
		display.display();
	}
	for (int16_t i=display.height()-1; i>=0; i-=4) {
		display.drawLine(display.width()-1, display.height()-1, 0, i, WHITE);
		display.display();
	}
	delay(250);

	display.clearDisplay();
	for (int16_t i=0; i<display.height(); i+=4) {
		display.drawLine(display.width()-1, 0, 0, i, WHITE);
		display.display();
	}
	for (int16_t i=0; i<display.width(); i+=4) {
		display.drawLine(display.width()-1, 0, i, display.height()-1, WHITE); 
		display.display();
	}
	delay(250);
}

void testscrolltext(void) {
	display.setTextSize(2);
	display.setTextColor(WHITE);
	display.setCursor(10,0);
	display.clearDisplay();
	display.println("scroll");
	display.display();
 
	display.startscrollright(0x00, 0x0F);
	delay(2000);
	display.stopscroll();
	delay(1000);
	display.startscrollleft(0x00, 0x0F);
	delay(2000);
	display.stopscroll();
	delay(1000);    
	display.startscrolldiagright(0x00, 0x07);
	delay(2000);
	display.startscrolldiagleft(0x00, 0x07);
	delay(2000);
	display.stopscroll();
}


*/


#endif		// #if defined USE_KSDisplay || defined KSLIBRARIES_USEALL