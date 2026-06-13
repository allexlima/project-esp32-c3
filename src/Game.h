#ifndef GAME_H
#define GAME_H

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include "Sprites.h"

#define SCREEN_W 128
#define SCREEN_H 128
#define GROUND_Y 112 // 128 - 16

#define NUM_PIPES 2

struct Pipe {
    float x;
    bool active;
};

class Game {
  private:
    Adafruit_ST7735& tft;

    // Mario Physics
    float marioY;
    float marioVY;
    bool isGrounded;
    
    // Physics Constants
    const float gravity = 0.6;
    const float jumpForce = -6.5;
    const float gameSpeed = 2.0;

    // Entities
    Pipe pipes[NUM_PIPES];
    
    // World
    float bgX;

    // Game State
    bool isGameOver;
    int score;

    // Timing
    unsigned long lastFrameTime;

    void drawSprite(int x, int y, const uint16_t* sprite);
    void resetGame();
    void spawnPipe(int index);

  public:
    Game(Adafruit_ST7735& display);

    void begin();
    void update();

    // Inputs
    void jump();
    void fireball(); // To be implemented, currently just visual
};

#endif
