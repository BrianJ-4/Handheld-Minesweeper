#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "LEDController.h"
#include <Adafruit_Keypad.h>

#define ROWS 10
#define COLS 6
#define NUM_MINES 9
#define LONG_PRESS_THRESHOLD 250

class Cell {
public:
    // Status types: unexplored, explored, flagged
    // TODO: Convert status types to enums
    Cell() : isMine(false), adjacentMines(0), status("unexplored") {}
    bool isMine;
    int adjacentMines;
    String status;
};

class GameController {
private:
    Cell grid[ROWS][COLS];
    LEDController& ledController;
    Adafruit_Keypad& keypad;

public:
    GameController(LEDController& controller, Adafruit_Keypad& keypad);
    void startGame();
    void generateGrid(int firstRow, int firstCol);
    void playGame();
    void getInput(int &row, int &col, bool &shortPress);
    void markCell(int row, int col, bool shortPress);
    bool checkMine(int row, int col);
    void updateLEDs();
    void recursiveClear(int row, int col);
    bool checkGameWin();
    void showWin();
    void showLoss();
    void printGrid(); // Debug Method
};

#endif