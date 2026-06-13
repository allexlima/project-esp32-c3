#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

// Custom Classes
#include "Button.h"
#include "Game.h"

// ---------------------------------------------------------
// Hardware Configuration
// ---------------------------------------------------------
// Display Pins
#define TFT_MOSI  4
#define TFT_SCLK  3
#define TFT_CS    2
#define TFT_DC    0
#define TFT_RST   5

// Button Pins
#define K1_PIN    8
#define K2_PIN    10

// ---------------------------------------------------------
// Global Objects
// ---------------------------------------------------------
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
Game superLeco(tft);

Button btnK1(K1_PIN);
Button btnK2(K2_PIN);

// ---------------------------------------------------------
// Setup
// ---------------------------------------------------------
void setup() {
  Serial.begin(115200);

  // Initialize buttons
  btnK1.begin();
  btnK2.begin();

  // Initialize Display
  SPI.begin(TFT_SCLK, -1, TFT_MOSI, TFT_CS);
  tft.initR(INITR_144GREENTAB);
  tft.setRotation(2);

  // Initialize Game
  superLeco.begin();

  Serial.println("System Initialized! Ready for Super Leco Room.");
}

// ---------------------------------------------------------
// Main Loop
// ---------------------------------------------------------
void loop() {
  // 1. Handle Input
  if (btnK1.wasPressed()) {
    superLeco.jump();
  }
  
  if (btnK2.wasPressed()) {
    superLeco.fireball();
  }

  // 2. Render next frame
  superLeco.update();
}
