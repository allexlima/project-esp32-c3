# ESP32-C3 Animated "Hello World" 🚀

A cool, non-blocking 60 FPS bouncing "Hello World" animation with a moving starfield, built in C++ for the **ESP32-C3 DevKitM-1** with an **ST7735 128x128 TFT display**.

## 🛠️ Hardware Setup

- **Board:** ESP32-C3 DevKitM-1
- **Display:** 1.44" ST7735 TFT (128x128 pixels, Green Tab)
- **Buttons:** K1 (GPIO 8) and K2 (GPIO 10)

### Wiring (SPI)
| TFT Pin | ESP32-C3 Pin |
|---------|--------------|
| `SDA / MOSI` | GPIO 4 |
| `SCL / SCK`  | GPIO 3 |
| `CS`         | GPIO 2 |
| `DC`         | GPIO 0 |
| `RST`        | GPIO 5 |

## ✨ Features

- **Smooth Animation:** Uses `millis()` for non-blocking 60 FPS frame timing (no `delay()` loops).
- **Collision Detection:** Text bounces dynamically off the screen edges.
- **Starfield:** Background particle system creating a sense of depth and motion.
- **Interactive:**
  - **Press K1 (IO8):** Cycles the text color.
  - **Press K2 (IO10):** Toggles animation speed (1.0x ➔ 2.0x ➔ 0.5x).

## 🚀 How to Run

1. **Install PlatformIO:** Install the [PlatformIO IDE extension](https://platformio.org/install/ide?install=vscode) in VS Code.
2. **Open the Project:** Open this repository folder in VS Code.
3. **Build & Upload:** 
   - Click the PlatformIO **Upload** button (the `→` arrow in the bottom status bar).
   - *Note: You may need to manually put the board into bootloader mode: Hold `BOOT`, press `RST`, release `BOOT`.*
4. **Enjoy!** The screen will initialize and the animation will start immediately.
