#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

class Button {
  private:
    uint8_t pin;
    bool lastState;
    bool currentState;
    unsigned long lastDebounceTime;
    unsigned long debounceDelay = 50;

  public:
    Button(uint8_t p);
    void begin();
    
    // Returns true if the button was just pressed (falling edge)
    bool wasPressed();
};

#endif
