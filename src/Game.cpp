#include "Game.h"

Game::Game(Adafruit_ST7735& display) : tft(display) {
    currentState = STATE_TITLE;
    stateJustChanged = true;
    isSprinting = false;
    resetGame();
}

void Game::resetGame() {
    marioY = GROUND_Y - 16;
    marioVY = 0;
    isGrounded = true;
    bgX = 0;
    score = 0;
    lastFrameTime = 0;

    for (int i = 0; i < NUM_ENEMIES; i++) {
        enemies[i].active = false;
    }
    // Initialize scenery
    bgScenery[0] = {SCREEN_W + 10, 20, 0}; // Cloud
    bgScenery[1] = {SCREEN_W + 80, 40, 0}; // Cloud
    bgScenery[2] = {SCREEN_W + 40, GROUND_Y - 16, 1}; // Bush
}

void Game::spawnEnemy(int index) {
    enemies[index].x = SCREEN_W + random(20, 80);
    enemies[index].type = random(0, 2); // Randomly choose 0 or 1
    enemies[index].active = true;
}

void Game::begin() {
    tft.fillScreen(C_SKY); 
    for (int i = 0; i < SCREEN_W; i += 16) {
        tft.drawRGBBitmap(i, GROUND_Y, brickSprite, 16, 16);
    }
}

void Game::setSprint(bool sprinting) {
    isSprinting = sprinting;
}

void Game::jump() {
    if (currentState == STATE_TITLE || currentState == STATE_GAMEOVER) {
        currentState = STATE_PLAYING;
        stateJustChanged = true;
        return;
    }
    
    if (currentState == STATE_PLAYING && isGrounded) {
        marioVY = jumpForce;
        isGrounded = false;
    }
}

void Game::fireball() {
    if (currentState == STATE_TITLE || currentState == STATE_GAMEOVER) {
        currentState = STATE_PLAYING;
        stateJustChanged = true;
        return;
    }
}

void Game::drawSprite(int x, int y, const uint16_t* sprite) {
    if (x <= -16 || x >= SCREEN_W) return;
    uint16_t buffer[256];
    int k = 0;
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            uint16_t color = pgm_read_word(&sprite[k++]);
            if (color != C_TRANS) {
                buffer[i*16 + j] = color;
            } else {
                buffer[i*16 + j] = C_SKY; 
            }
        }
    }
    tft.drawRGBBitmap(x, y, buffer, 16, 16);
}

void Game::drawTintedSprite(int x, int y, const uint16_t* sprite, uint16_t tintColor) {
    if (x <= -16 || x >= SCREEN_W) return;
    uint16_t buffer[256];
    int k = 0;
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            uint16_t color = pgm_read_word(&sprite[k++]);
            if (color == C_WHT) { 
                buffer[i*16 + j] = tintColor;
            } else {
                buffer[i*16 + j] = C_SKY;
            }
        }
    }
    tft.drawRGBBitmap(x, y, buffer, 16, 16);
}

void Game::drawTitleScreen() {
    tft.fillRect(0, 0, SCREEN_W, GROUND_Y, C_SKY);
    
    // Draw Logo Frame
    tft.fillRoundRect(8, 10, SCREEN_W - 16, 45, 8, ST77XX_RED);
    tft.drawRoundRect(8, 10, SCREEN_W - 16, 45, 8, ST77XX_WHITE);
    tft.drawRoundRect(9, 11, SCREEN_W - 18, 43, 7, ST77XX_YELLOW); // Double border

    // SUPER text with drop shadow
    tft.setTextSize(2);
    tft.setCursor(20, 18);
    tft.setTextColor(ST77XX_BLACK); // shadow
    tft.print("SUPER");
    tft.setCursor(18, 16);
    tft.setTextColor(ST77XX_WHITE);
    tft.print("SUPER");

    // LECO ROOM text with drop shadow
    tft.setTextSize(1);
    tft.setCursor(38, 38);
    tft.setTextColor(ST77XX_BLACK); // shadow
    tft.print("LECO ROOM");
    tft.setCursor(37, 37);
    tft.setTextColor(ST77XX_YELLOW);
    tft.print("LECO ROOM");

    // Instructions
    tft.setCursor(15, 65);
    tft.setTextColor(ST77XX_YELLOW);
    tft.print("K1: JUMP");
    tft.setCursor(15, 80);
    tft.print("K2: SPRINT (Hold)");

    // Blink Start Prompt
    if ((millis() / 500) % 2 == 0) {
        tft.setCursor(20, 100);
        tft.setTextColor(ST77XX_WHITE);
        tft.print("PRESS K1 TO START");
    }
}

void Game::update() {
    unsigned long currentTime = millis();
    if (currentTime - lastFrameTime < 16) return; // ~60 FPS
    lastFrameTime = currentTime;

    if (currentState == STATE_TITLE) {
        if (stateJustChanged) {
            begin(); // Draw background
            stateJustChanged = false;
        }
        drawTitleScreen();
        return;
    }

    if (currentState == STATE_GAMEOVER) {
        if (stateJustChanged) {
            tft.setCursor(10, 40);
            tft.setTextColor(ST77XX_WHITE, ST77XX_RED);
            tft.setTextSize(2);
            tft.print("GAME OVER");
            
            tft.setTextSize(1);
            tft.setCursor(10, 65);
            tft.setTextColor(ST77XX_WHITE, C_SKY);
            tft.print("Score: ");
            tft.print(score);
            
            tft.setCursor(15, 85);
            tft.print("PRESS K1 TO RETRY");
            stateJustChanged = false;
        }
        return;
    }

    // STATE_PLAYING
    if (stateJustChanged) {
        resetGame();
        begin();
        spawnEnemy(0);
        enemies[0].x = SCREEN_W;
        stateJustChanged = false;
    }

    float currentSpeed = isSprinting ? gameSpeed * 1.5 : gameSpeed;

    // --- PHYSICS ---
    marioVY += gravity;
    marioY += marioVY;

    if (marioY >= GROUND_Y - 16) {
        marioY = GROUND_Y - 16;
        marioVY = 0;
        isGrounded = true;
    }

    // Move Enemies (Goombas)
    for (int i = 0; i < NUM_ENEMIES; i++) {
        if (enemies[i].active) {
            enemies[i].x -= currentSpeed;
            if (enemies[i].x < -16) {
                spawnEnemy(i);
                score++;
            }
            
            // Collision Detection
            if (enemies[i].x < 20 + 12 && enemies[i].x + 12 > 20) { 
                if (marioY + 14 > GROUND_Y - 16) { 
                    currentState = STATE_GAMEOVER;
                    stateJustChanged = true;
                }
            }
        }
    }

    // Move Scenery
    for (int i = 0; i < 3; i++) {
        bgScenery[i].x -= (currentSpeed * 0.5); 
        if (bgScenery[i].x < -16) {
            bgScenery[i].x = SCREEN_W + random(10, 50);
            if (bgScenery[i].type == 0) bgScenery[i].y = random(10, 50);
        }
    }

    // Scroll floor
    bgX -= currentSpeed;
    if (bgX <= -16) bgX += 16;

    // --- RENDERING ---
    tft.fillRect(0, 0, SCREEN_W, GROUND_Y, C_SKY);

    // Draw Scenery
    for (int i = 0; i < 3; i++) {
        if (bgScenery[i].type == 0) {
            drawSprite((int)bgScenery[i].x, bgScenery[i].y, cloudSprite); 
        } else {
            drawTintedSprite((int)bgScenery[i].x, bgScenery[i].y, cloudSprite, 0x0520); 
        }
    }

    // Draw Mario
    drawSprite(20, (int)marioY, marioSprite);

    // Draw Enemies (Goombas or Pipes)
    for (int i = 0; i < NUM_ENEMIES; i++) {
        if (enemies[i].active) {
            if (enemies[i].type == 0) {
                drawSprite((int)enemies[i].x, GROUND_Y - 16, goombaSprite); // Goomba
            } else {
                drawSprite((int)enemies[i].x, GROUND_Y - 16, pipeTopSprite); // Pipe
                tft.fillRect((int)enemies[i].x + 2, GROUND_Y, 12, SCREEN_H - GROUND_Y, C_GREN); // Pipe body
            }
        }
    }

    // Draw Scrolling Floor Bricks
    for (int i = bgX; i < SCREEN_W; i += 16) {
        tft.drawRGBBitmap(i, GROUND_Y, brickSprite, 16, 16);
    }

    // Draw Score
    tft.setCursor(2, 2);
    tft.setTextColor(ST77XX_WHITE, C_SKY); 
    tft.setTextSize(1);
    tft.print("Score: ");
    tft.print(score);
}
