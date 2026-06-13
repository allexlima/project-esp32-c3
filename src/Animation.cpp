#include "Animation.h"

const uint16_t Animation::colors[6] = {
  ST77XX_YELLOW, ST77XX_CYAN, ST77XX_MAGENTA, 
  ST77XX_GREEN, ST77XX_ORANGE, ST77XX_WHITE
};

Animation::Animation(Adafruit_ST7735& display) 
  : tft(display),
    textX(10), textY(64),
    textVelX(1.5), textVelY(1.0),
    textSize(2), textWidth(12 * 6), textHeight(16),
    colorIndex(0),
    speedMultiplier(1.0), speedState(0),
    lastFrameTime(0), targetFPS(60), frameDelay(1000 / 60) {
}

void Animation::begin() {
  for (int i = 0; i < NUM_STARS; i++) {
    stars[i].x = random(0, SCREEN_WIDTH);
    stars[i].y = random(0, SCREEN_HEIGHT);
    stars[i].speed = random(1, 4) * 0.5;
  }
}

void Animation::changeSpeed() {
  speedState = (speedState + 1) % 3;
  if (speedState == 0) speedMultiplier = 1.0;
  else if (speedState == 1) speedMultiplier = 2.0; // Fast
  else speedMultiplier = 0.5; // Slow
}

void Animation::changeColor() {
  colorIndex = (colorIndex + 1) % (sizeof(colors) / sizeof(colors[0]));
}

void Animation::update() {
  unsigned long currentTime = millis();
  if (currentTime - lastFrameTime >= frameDelay) {
    lastFrameTime = currentTime;

    // Erase Previous Frame
    tft.fillScreen(ST77XX_BLACK);

    // Update & Draw Stars
    for (int i = 0; i < NUM_STARS; i++) {
      stars[i].y += (stars[i].speed * speedMultiplier);
      if (stars[i].y >= SCREEN_HEIGHT) {
        stars[i].y = 0;
        stars[i].x = random(0, SCREEN_WIDTH);
      }
      tft.drawPixel((int)stars[i].x, (int)stars[i].y, ST77XX_WHITE);
    }

    // Update Text Position
    textX += (textVelX * speedMultiplier);
    textY += (textVelY * speedMultiplier);

    // Collision Detection (Bounce off edges)
    bool bounced = false;
    if (textX <= 0) {
      textX = 0;
      textVelX = abs(textVelX);
      bounced = true;
    } else if (textX + textWidth >= SCREEN_WIDTH) {
      textX = SCREEN_WIDTH - textWidth;
      textVelX = -abs(textVelX);
      bounced = true;
    }

    if (textY <= 0) {
      textY = 0;
      textVelY = abs(textVelY);
      bounced = true;
    } else if (textY + textHeight >= SCREEN_HEIGHT) {
      textY = SCREEN_HEIGHT - textHeight;
      textVelY = -abs(textVelY);
      bounced = true;
    }

    if (bounced) {
      changeColor(); // Auto-change color on bounce
    }

    // Draw Text
    tft.setTextSize(textSize);
    tft.setTextColor(colors[colorIndex]);
    
    // Draw "Hello"
    tft.setCursor((int)textX, (int)textY);
    tft.print("Hello");
    
    // Draw "World" just below it
    tft.setCursor((int)textX + 5, (int)textY + 16);
    tft.print("World");
  }
}
