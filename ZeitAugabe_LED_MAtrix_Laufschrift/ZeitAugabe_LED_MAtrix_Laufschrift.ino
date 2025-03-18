#include <WiFi.h>
#include "time.h"
#include <FastLED.h>
#include <LEDMatrix.h>
#include <LEDText.h>
#include <FontMatrise.h>

const char* ssid     = "iPhone von Leon";  
const char* password = "herwigNattl123";  

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 3600;

// LED-Matrix Konfiguration
#define LED_PIN        32
#define COLOR_ORDER    GRB
#define CHIPSET        WS2812B

#define MATRIX_WIDTH   -32
#define MATRIX_HEIGHT  8
#define MATRIX_TYPE    VERTICAL_ZIGZAG_MATRIX

cLEDMatrix<MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_TYPE> leds;
cLEDText ScrollingMsg;

bool setTextFlag = false;

void setup() {
  Serial.begin(115200);

  // WLAN-Verbindung aufbauen
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi verbunden.");

  // NTP-Zeit abrufen
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  // LED-Matrix initialisieren
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds[0], leds.Size());
  FastLED.setBrightness(8);
  FastLED.clear(true);
  delay(500);
  FastLED.show();

  ScrollingMsg.SetFont(MatriseFontData);
  ScrollingMsg.Init(&leds, leds.Width(), ScrollingMsg.FontHeight() + 1, 0, 0);
  ScrollingMsg.SetTextColrOptions(COLR_RGB | COLR_SINGLE, 0x00, 0xff, 0xff);  // Cyan
  
}

void loop() {
  // Zeit abrufen
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Zeit konnte nicht abgerufen werden");
    return;
  }

  // Stunden, Minuten und Sekunden formatieren
  char timeString[14];  
  strftime(timeString, sizeof(timeString), "     %H:%M:%S", &timeinfo);
  Serial.println(timeString);

  // Zeit auf der LED-Matrix anzeigen
  if (!setTextFlag) {
    ScrollingMsg.SetText((unsigned char *)timeString, strlen(timeString));
    setTextFlag = true;
  }
  
  if (ScrollingMsg.UpdateText() == -1) {
    ScrollingMsg.SetText((unsigned char *)timeString, strlen(timeString));
  } else {
    FastLED.show();
  }
  
  delay(100);
}