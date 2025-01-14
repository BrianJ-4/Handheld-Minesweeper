#include "LEDController.h"

LEDController::LEDController() : strip(NUM_PIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800) {
    strip.begin();
    strip.setBrightness(20);
    strip.show();
    turnOffAllPixels();
}

uint32_t LEDController::getRandomColor() {
    int red = random(0, 256);
    int green = random(0, 256);
    int blue = random(0, 256);
    return strip.Color(red, green, blue);
}

uint32_t LEDController::getColorFromName(Color colorName) {
    switch (colorName) {
        case RED:     return strip.Color(255, 0, 0);
        case GREEN:   return strip.Color(0, 255, 0);
        case BLUE:    return strip.Color(0, 0, 255);
        case WHITE:   return strip.Color(255, 255, 255);
        case YELLOW:  return strip.Color(255, 255, 0);
        case CYAN:    return strip.Color(0, 255, 255);
        case MAGENTA: return strip.Color(255, 0, 255);
        case PURPLE: return strip.Color(128, 0, 128);
        case ORANGE: return strip.Color(255, 165, 0);
        default:      return strip.Color(0, 0, 0);
    }
}

void LEDController::playEffect(const char *sequence) {
    for (int i = 0; i < strlen(sequence); i++) {
        char currentCharacter = sequence[i];
        int pixelIndex = strchr(indexMapping, currentCharacter) - indexMapping;
        int row = pixelIndex / COLS;
        int col = pixelIndex % COLS;

        int ledIndex = ledIndexMap[row][col];
        strip.setPixelColor(ledIndex, getRandomColor());
        strip.show();
        delay(50);
        strip.setPixelColor(ledIndex, strip.Color(0, 0, 0));
        strip.show();
    }
    turnOffAllPixels();
}

void LEDController::setPixel(char index, Color color) {
    uint32_t colorVal = getColorFromName(color);
    int pixelIndex = strchr(indexMapping, index) - indexMapping;
    int row = pixelIndex / COLS;
    int col = pixelIndex % COLS;

    int ledIndex = ledIndexMap[row][col];
    strip.setPixelColor(ledIndex, colorVal);
    strip.show();
}

void LEDController::turnOffPixel(char index) {
    int pixelIndex = strchr(indexMapping, index) - indexMapping;
    int row = pixelIndex / COLS;
    int col = pixelIndex % COLS;

    int ledIndex = ledIndexMap[row][col];
    strip.setPixelColor(ledIndex, strip.Color(0, 0, 0));
    strip.show();
}

void LEDController::turnOffAllPixels() {
    for (int i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, 0);
    }
    strip.show();
}