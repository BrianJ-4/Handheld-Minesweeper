#include "GameController.h"

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

struct KeyTiming {
  unsigned long pressStart = 0;
  bool isLongPress = false;
};

KeyTiming keyTimings[ROWS][COLS];

const char *indexMapping = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwx";

GameController::GameController(LEDController& controller, Adafruit_Keypad& keypad) : ledController(controller), keypad(keypad) {
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            grid[r][c] = Cell();
        }
    }
}

void GameController::startGame() {
    ledController.setAllPixels(LEDController::GREEN);
    int row;
    int col;
    bool shortPress;
    getInput(row, col, shortPress);
    updateLEDs();
    generateGrid(row, col);
    recursiveClear(row, col);
    markCell(row, col, true);
    updateLEDs();
    playGame();
}

void GameController::generateGrid(int firstRow, int firstCol) {
    for (int i = 0; i < NUM_MINES; i++) {
        int r = random(0, ROWS);
        int c = random(0, COLS);

        if (grid[r][c].isMine || (abs(r - firstRow) <= 1 && abs(c - firstCol) <= 1)) {
            i--; // Decrement i so NUM_MINES will still be placed
            continue;
        }

        grid[r][c].isMine = true;

        // Update adjacentMines value for the mines adjacent cells
        for (int rowOffset = -1; rowOffset <= 1; rowOffset++) {
            for (int colOffset = -1; colOffset <= 1; colOffset++) {
                int adjacentRow = r + rowOffset;
                int adjacentCol = c + colOffset;
                // Make sure adjacent cell is within bounds and not a mine
                if (adjacentRow >= 0 && adjacentRow < ROWS && adjacentCol >= 0 && adjacentCol < COLS && !grid[adjacentRow][adjacentCol].isMine) {
                    grid[adjacentRow][adjacentCol].adjacentMines++;
                }
            }
        }
    }
}


void GameController::getInput(int &row, int &col, bool &shortPress) {
    while (true) {
        keypad.tick();
        while (keypad.available()) {
            keypadEvent e = keypad.read();
            row = e.bit.ROW;
            col = COLS - 1 - e.bit.COL;
            char symbol = keys[row][col];

            if (e.bit.EVENT == KEY_JUST_PRESSED) {
                keyTimings[row][col].pressStart = millis();
                keyTimings[row][col].isLongPress = false;
            }
            else if (e.bit.EVENT == KEY_JUST_RELEASED) {
                unsigned long pressDuration = millis() - keyTimings[row][col].pressStart;
                if (pressDuration >= LONG_PRESS_THRESHOLD)
                    shortPress = false;
                else
                    shortPress = true;
                return;
            }
        }
        delay(10);
    }
}

void GameController::playGame() {
    int row;
    int col;
    bool shortPress;
    while (true)
    {
        getInput(row, col, shortPress);
        if (!shortPress) {
            markCell(row, col, false);
        }
        else {
            if(checkMine(row, col))
                break;
        }
        // Update LEDs here based on the changes in cells
        updateLEDs();

        if(checkGameWin()) {
            showWin();
        }
    }
    showLoss();
}

bool GameController::checkMine(int row, int col) {
    if (grid[row][col].isMine)
        return true;
    else {
        recursiveClear(row, col);
        markCell(row, col, true);
        return false;
    }
}

void GameController::markCell(int row, int col, bool shortPress) {
    if (shortPress)
        grid[row][col].status = "explored";
    else
        grid[row][col].status = "flagged";
}

void GameController::printGrid() {
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            if (grid[row][col].isMine) {
                Serial.print("*");
            } else {
                Serial.print(grid[row][col].adjacentMines);
            }
        }
        Serial.println();
    }
}

void GameController::updateLEDs() {
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            // If unexplored set green
            if (grid[r][c].status == "unexplored") {
                ledController.setPixel(keys[r][c], LEDController::GREEN);
            }
            // If explored set color to represent number of adjacent mines
            if (grid[r][c].status == "explored") {
                if (grid[r][c].adjacentMines == 0)
                    ledController.setPixel(keys[r][c], LEDController::WHITE);
                else if (grid[r][c].adjacentMines == 1)
                    ledController.setPixel(keys[r][c], LEDController::BLUE);
                else if (grid[r][c].adjacentMines == 2)
                    ledController.setPixel(keys[r][c], LEDController::ORANGE);
                else if (grid[r][c].adjacentMines == 3)
                    ledController.setPixel(keys[r][c], LEDController::MAGENTA);
                else if (grid[r][c].adjacentMines == 4)
                    ledController.setPixel(keys[r][c], LEDController::CYAN);
            }
            // If flagged set red
            else if (grid[r][c].status == "flagged")
                ledController.setPixel(keys[r][c], LEDController::RED);
        }
    }
}

void GameController::recursiveClear(int row, int col) {
    // Make sure cell is in grid
    // Serial.println("Row: " + String(row) + " Col: " + String(col));
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS)
        return;
    // Serial.println("HIT 2: " + grid[row][col].status);
    //Check if cell has already been cleared or flagged
    if (grid[row][col].status == "explored" || grid[row][col].status == "flagged")
        return;
    // Serial.println("HIT 3");
    markCell(row, col, true);

    // Stop if cell has adjacent mines
    if (grid[row][col].adjacentMines != 0) {
        return;
    }
    for (int rowOffset = -1; rowOffset <= 1; rowOffset++) {
            for (int colOffset = -1; colOffset <= 1; colOffset++) {
                if (rowOffset == 0 && colOffset == 0)
                    continue;
                int adjacentRow = row + rowOffset;
                int adjacentCol = col + colOffset;
                recursiveClear(adjacentRow, adjacentCol);
            }
        }
}

bool GameController::checkGameWin() {
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            if (grid[row][col].status == "unexplored" && !grid[row][col].isMine) {
                return false;
            }
        }
    }
    return true;
}

void GameController::showWin() {
    Serial.println("Game Over");
    ledController.turnOffAllPixels();
    ledController.playEffect(ledController.zigZagSequence);
    ledController.setAllPixels(LEDController::GREEN);
}

void GameController::showLoss() {
    Serial.println("Game Over");
    ledController.setAllPixels(LEDController::RED);
}