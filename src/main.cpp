#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

// Pin definitions for ESP32-C3
#define TFT_MOSI  4
#define TFT_SCLK  3
#define TFT_CS    2
#define TFT_DC    0
#define TFT_RST   5

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Starting Hello World...");

  // Initialize SPI with custom pins for ESP32-C3
  SPI.begin(TFT_SCLK, -1, TFT_MOSI, TFT_CS);

  // Initialize 128x128 ST7735 display (green tab variant)
  tft.initR(INITR_144GREENTAB);
  tft.setRotation(2);
  tft.fillScreen(ST77XX_BLUE);

  tft.setTextColor(ST77XX_YELLOW);
  tft.setTextSize(2);
  tft.setCursor(10, 50);
  tft.println("Hello");
  tft.setCursor(10, 70);
  tft.println("World!");

  Serial.println("Display initialized.");
}

void loop() {
  // Blink onboard LED on GPIO 8
  static bool ledInit = false;
  if (!ledInit) {
    pinMode(8, OUTPUT);
    ledInit = true;
  }

  digitalWrite(8, HIGH);
  delay(500);
  digitalWrite(8, LOW);
  delay(500);
  Serial.println("Tick...");
}
