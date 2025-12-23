#ifndef LEDCONTROLLER_H
#define LEDCONTROLLER_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "Settings.h"
#include "GridMap.h"

#define LED_PIN 6      // Common pin for ESP32-C3 SuperMini (check your board!)
#define WIDTH 11
#define HEIGHT 10
#define NUM_LEDS_GRID  119
#define NUM_LEDS_TOTAL 123

class LedController {
public:
    LedController(Settings* settings);
    void begin();
    void loop(); // Call in main loop

    void applySettings(); // Re-read brightness/color from settings

private:
    Settings* _settings;
    Adafruit_NeoPixel strip;
    
    // State
    uint32_t lastUpdate = 0;
    bool displayingDate = false;
    uint32_t dateStartTime = 0;
    
    // Helpers
    int getPixelIndex(int x, int y);
    void clear();
    void show();
    void setPixel(int i, uint32_t c);
    void setWord(Word w, uint32_t c);
    void setCorner(int minutes, uint32_t c);
    
    // Logic
    void updateTimeDisplay();
    void updateDateDisplay();
    void matrixDate(const int place[], const bool number[], int r, int g, int b);
    void showStartupAnimation();
    
    // Logic helpers
    uint32_t getCurrentColor();
    int getCurrentBrightness();

    // test
    void testAllWords();
    void testAllDates();
    void testAllCorners();
};

#endif
