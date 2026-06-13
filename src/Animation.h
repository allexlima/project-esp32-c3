#ifndef ANIMATION_H
#define ANIMATION_H

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 128
#define NUM_STARS     20

struct Star {
  float x, y, speed;
};

class Animation {
  private:
    Adafruit_ST7735& tft;

    // Bouncing Text State
    float textX;
    float textY;
    float textVelX;
    float textVelY;
    int textSize;
    int textWidth;
    int textHeight;
    
    int colorIndex;
    static const uint16_t colors[6];

    // Speed State
    float speedMultiplier;
    int speedState; // 0 = Normal, 1 = Fast, 2 = Slow

    // Starfield State
    Star stars[NUM_STARS];

    // Frame Timing
    unsigned long lastFrameTime;
    int targetFPS;
    int frameDelay;

  public:
    // Constructor takes a reference to the display object
    Animation(Adafruit_ST7735& display);

    // Initialize animation state
    void begin();

    // Call this inside the main loop
    void update();

    // Interactions
    void changeSpeed();
    void changeColor();
};

#endif
