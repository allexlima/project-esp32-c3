#include "Game.h"

Game::Game(Adafruit_ST7735& display) : tft(display) {
    resetGame();
}

void Game::resetGame() {
    marioY = GROUND_Y - 16;
    marioVY = 0;
    isGrounded = true;
    bgX = 0;
    score = 0;
    isGameOver = false;
    lastFrameTime = 0;

    for (int i = 0; i < NUM_PIPES; i++) {
        pipes[i].active = false;
    }
    // Spawn first pipe
    spawnPipe(0);
    pipes[0].x = SCREEN_W;
}

void Game::spawnPipe(int index) {
    pipes[index].x = SCREEN_W + random(20, 80);
    pipes[index].active = true;
}

void Game::begin() {
    tft.fillScreen(0x5CDF); // Light blue sky color
    tft.fillRect(0, GROUND_Y, SCREEN_W, SCREEN_H - GROUND_Y, C_BRWN); // Ground
}

void Game::jump() {
    if (isGameOver) {
        resetGame();
        begin();
        return;
    }
    
    if (isGrounded) {
        marioVY = jumpForce;
        isGrounded = false;
    }
}

void Game::fireball() {
    if (isGameOver) return;
    // TODO: Implement fireball logic
}

void Game::drawSprite(int x, int y, const uint16_t* sprite) {
    // Basic sprite drawing loop (ignores transparent pixels)
    int k = 0;
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            uint16_t color = pgm_read_word(&sprite[k++]);
            if (color != C_TRANS) {
                tft.drawPixel(x + j, y + i, color);
            }
        }
    }
}

void Game::update() {
    unsigned long currentTime = millis();
    if (currentTime - lastFrameTime < 16) return; // ~60 FPS
    lastFrameTime = currentTime;

    if (isGameOver) {
        tft.setCursor(20, 40);
        tft.setTextColor(ST77XX_RED);
        tft.setTextSize(2);
        tft.print("GAME OVER");
        return;
    }

    // --- PHYSICS ---
    // Apply gravity
    marioVY += gravity;
    marioY += marioVY;

    // Floor collision
    if (marioY >= GROUND_Y - 16) {
        marioY = GROUND_Y - 16;
        marioVY = 0;
        isGrounded = true;
    }

    // Move pipes
    for (int i = 0; i < NUM_PIPES; i++) {
        if (pipes[i].active) {
            pipes[i].x -= gameSpeed;
            if (pipes[i].x < -16) {
                spawnPipe(i);
                score++;
            }
            
            // Basic collision logic (Bounding Box)
            // Mario is at x=20, width=16, y=marioY, height=16
            // Pipe is at x=pipes[i].x, width=16, y=GROUND_Y-16, height=16
            if (pipes[i].x < 20 + 12 && pipes[i].x + 12 > 20) { // X overlap
                if (marioY + 14 > GROUND_Y - 16) { // Y overlap (hit the pipe)
                    isGameOver = true;
                }
            }
        }
    }

    // --- RENDERING ---
    // Instead of full clear (which flickers), we draw a solid rect over where Mario WAS,
    // but calculating dirty rects is better. For now, clear the "sky" area quickly:
    tft.fillRect(0, 0, SCREEN_W, GROUND_Y, 0x5CDF);

    // Draw Mario
    drawSprite(20, (int)marioY, marioSprite);

    // Draw Pipes
    for (int i = 0; i < NUM_PIPES; i++) {
        if (pipes[i].active) {
            drawSprite((int)pipes[i].x, GROUND_Y - 16, pipeTopSprite);
            // Draw pipe body down to ground
            tft.fillRect((int)pipes[i].x + 2, GROUND_Y, 12, SCREEN_H - GROUND_Y, C_GREN);
        }
    }

    // Draw Score
    tft.setCursor(2, 2);
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(1);
    tft.print("Score: ");
    tft.print(score);
}
