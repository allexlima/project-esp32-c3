#include "Button.h"

Button::Button(uint8_t p) : pin(p), lastState(HIGH), currentState(HIGH), lastDebounceTime(0) {}

void Button::begin() {
  pinMode(pin, INPUT_PULLUP);
}

bool Button::wasPressed() {
  bool reading = digitalRead(pin);
  bool pressed = false;

  if (reading != lastState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != currentState) {
      currentState = reading;
      if (currentState == LOW) { // Assuming pull-up (LOW = pressed)
        pressed = true;
      }
    }
  }
  lastState = reading;
  return pressed;
}

bool Button::isPressed() {
  // Simple check, returns true if currently LOW (held down)
  return digitalRead(pin) == LOW;
}
