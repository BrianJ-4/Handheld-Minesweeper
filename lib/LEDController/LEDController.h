#ifndef LEDCONTROLLER_H
#define LEDCONTROLLER_H

#include <Adafruit_NeoPixel.h>

class LEDController
{
public:
    enum Color
    {
        RED,
        GREEN,
        BLUE,
        WHITE,
        YELLOW,
        CYAN,
        MAGENTA,
        PURPLE,
        ORANGE
    };
    const char *zigZagSequence = "012345BA9876CDEFGHNMLKJIOPQRSZYXWVUabcdeflkjihgmnopqrxwvuts";
    const char *spiralSequence = "QWXRLKJPVbcdeYSMGFEDCIOUaghijklfZTNH";

private:
    static const uint8_t ROWS = 10;
    static const uint8_t COLS = 6;
    uint8_t rowPins[ROWS] = {16, 4, 2, 13, 32, 33, 25, 26, 27, 14};
    uint8_t colPins[COLS] = {22, 5, 17, 21, 19, 18};

    static const uint8_t NEOPIXEL_PIN = 23;
    static const uint16_t NUM_PIXELS = ROWS * COLS;
    Adafruit_NeoPixel strip;

    const int ledIndexMap[ROWS][COLS] = {
        {0, 1, 2, 3, 4, 5},
        {11, 10, 9, 8, 7, 6},
        {12, 13, 14, 15, 16, 17},
        {23, 22, 21, 20, 19, 18},
        {24, 25, 26, 27, 28, 29},
        {30, 31, 32, 33, 34, 35},
        {41, 40, 39, 38, 37, 36},
        {42, 43, 44, 45, 46, 47},
        {53, 52, 51, 50, 49, 48},
        {54, 55, 56, 57, 58, 59}};

    const char *indexMapping = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwx";

    uint32_t getRandomColor();
    uint32_t getColorFromName(Color colorName);

public:
    LEDController();
    void playEffect(const char *sequence);
    void setPixel(char index, Color color);
    void setAllPixels(Color color);
    void turnOffPixel(char index);
    void turnOffAllPixels();
};

#endif