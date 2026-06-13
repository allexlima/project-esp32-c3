# Super Leco Room 🍄

A custom 2D auto-runner built entirely in C++ for the ESP32-C3!

## Features
* **Custom Engine:** Built from scratch using Adafruit GFX, utilizing `drawRGBBitmap` for buttery-smooth 60 FPS hardware SPI rendering.
* **Physics:** Real gravity curves, velocity, and bounding-box collision detection.
* **Parallax Scrolling:** Beautiful World 1-1 aesthetics with a moving tiled brick floor, clouds, and bushes.
* **Goombas:** Avoid the classic enemies as they scroll towards you!

## Controls
You only need the two buttons on your ESP32-C3 dev board:
* **K1 (IO8):** Press to Jump!
* **K2 (IO10):** Hold to Sprint! (Speeds up the game)

## How to Play
1. Compile and upload via **PlatformIO**.
2. At the Title Screen, press either **K1** or **K2** to start running.
3. Jump over the Goombas to increase your score! If you hit one, it's Game Over.
