#include <Arduino.h>
#include "LEDController.h"
#include "GameController.h"

#define ROWS 10
#define COLS 6

const char keys[ROWS][COLS] = {
  {'0', '1', '2', '3', '4', '5'},
  {'6', '7', '8', '9', 'A', 'B'},
  {'C', 'D', 'E', 'F', 'G', 'H'},
  {'I', 'J', 'K', 'L', 'M', 'N'},
  {'O', 'P', 'Q', 'R', 'S', 'T'},
  {'U', 'V', 'W', 'X', 'Y', 'Z'},
  {'a', 'b', 'c', 'd', 'e', 'f'},
  {'g', 'h', 'i', 'j', 'k', 'l'},
  {'m', 'n', 'o', 'p', 'q', 'r'},
  {'s', 't', 'u', 'v', 'w', 'x'}
};

uint8_t rowPins[ROWS] = {16, 4, 2, 13, 32, 33, 25, 26, 27, 14};
uint8_t colPins[COLS] = {22, 5, 17, 21, 19, 18};

LEDController *ledController;
Adafruit_Keypad *keypad;
GameController *game;

void setup() {
  Serial.begin(115200);
  ledController = new LEDController();
  keypad = new Adafruit_Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
  keypad->begin();
  game = new GameController(*ledController, *keypad);
  game->startGame();
}

void loop() {}