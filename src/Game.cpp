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

    // Initialize scenery
    bgScenery[0] = {SCREEN_W + 10, 20, 0}; // Cloud
    bgScenery[1] = {SCREEN_W + 80, 40, 0}; // Cloud
    bgScenery[2] = {SCREEN_W + 40, GROUND_Y - 16, 1}; // Bush
}

void Game::spawnPipe(int index) {
    pipes[index].x = SCREEN_W + random(20, 80);
    pipes[index].active = true;
}

void Game::begin() {
    tft.fillScreen(C_SKY); // Mario Blue Sky
    // Initial draw of the brick floor
    for (int i = 0; i < SCREEN_W; i += 16) {
        drawSprite(i, GROUND_Y, brickSprite);
    }
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

// Special draw for tinted sprites (bushes are just green clouds)
void Game::drawTintedSprite(int x, int y, const uint16_t* sprite, uint16_t tintColor) {
    int k = 0;
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            uint16_t color = pgm_read_word(&sprite[k++]);
            if (color == C_WHT) { // Replace white with tint
                tft.drawPixel(x + j, y + i, tintColor);
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
    marioVY += gravity;
    marioY += marioVY;

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
            if (pipes[i].x < 20 + 12 && pipes[i].x + 12 > 20) { 
                if (marioY + 14 > GROUND_Y - 16) { 
                    isGameOver = true;
                }
            }
        }
    }

    // Move Scenery
    for (int i = 0; i < 3; i++) {
        bgScenery[i].x -= (gameSpeed * 0.5); // Parallax scrolling (slower)
        if (bgScenery[i].x < -16) {
            bgScenery[i].x = SCREEN_W + random(10, 50);
            // Randomize height slightly for clouds
            if (bgScenery[i].type == 0) bgScenery[i].y = random(10, 50);
        }
    }

    // Scroll floor
    bgX -= gameSpeed;
    if (bgX <= -16) bgX += 16;

    // --- RENDERING ---
    // Clear sky
    tft.fillRect(0, 0, SCREEN_W, GROUND_Y, C_SKY);

    // Draw Scenery
    for (int i = 0; i < 3; i++) {
        if (bgScenery[i].type == 0) {
            drawSprite((int)bgScenery[i].x, bgScenery[i].y, cloudSprite); // Cloud
        } else {
            drawTintedSprite((int)bgScenery[i].x, bgScenery[i].y, cloudSprite, 0x0520); // Bush (dark green tint)
        }
    }

    // Draw Mario
    drawSprite(20, (int)marioY, marioSprite);

    // Draw Pipes
    for (int i = 0; i < NUM_PIPES; i++) {
        if (pipes[i].active) {
            drawSprite((int)pipes[i].x, GROUND_Y - 16, pipeTopSprite);
            tft.fillRect((int)pipes[i].x + 2, GROUND_Y, 12, SCREEN_H - GROUND_Y, C_GREN);
        }
    }

    // Draw Scrolling Floor Bricks
    for (int i = bgX; i < SCREEN_W; i += 16) {
        drawSprite(i, GROUND_Y, brickSprite);
    }

    // Draw Score
    tft.setCursor(2, 2);
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(1);
    tft.print("Score: ");
    tft.print(score);
}
