#ifndef GAME_H
#define GAME_H

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include "Sprites.h"

#define SCREEN_W 128
#define SCREEN_H 128
#define GROUND_Y 112 // 128 - 16

#define NUM_ENEMIES 2

struct Enemy {
    float x;
    bool active;
};

struct Scenery {
    float x;
    int y;
    int type; // 0 = cloud, 1 = bush
};

enum GameState {
    STATE_TITLE,
    STATE_PLAYING,
    STATE_GAMEOVER
};

class Game {
  private:
    Adafruit_ST7735& tft;

    // Game State
    GameState currentState;
    bool stateJustChanged;
    int score;
    bool isSprinting;

    // Mario Physics
    float marioY;
    float marioVY;
    bool isGrounded;
    
    // Physics Constants
    const float gravity = 0.6;
    const float jumpForce = -6.5;
    const float gameSpeed = 2.0;

    // Entities
    Enemy enemies[NUM_ENEMIES];
    Scenery bgScenery[3]; // 3 background objects (clouds/bushes)
    
    // World
    float bgX;

    // Timing
    unsigned long lastFrameTime;

    void drawSprite(int x, int y, const uint16_t* sprite);
    void drawTintedSprite(int x, int y, const uint16_t* sprite, uint16_t tintColor);
    void resetGame();
    void spawnEnemy(int index);
    void drawTitleScreen();

  public:
    Game(Adafruit_ST7735& display);

    void begin();
    void update();

    // Inputs
    void jump();
    void fireball();
    void setSprint(bool sprinting);
};

#endif
