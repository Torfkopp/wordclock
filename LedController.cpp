#include "LedController.h"
#include "TimeManager.h" // Needed for struct tm
#include "DateInfo.h"

extern TimeManager timeManager; // Need access to time

LedController::LedController(Settings* settings) 
    : _settings(settings), 
      strip(NUM_LEDS_TOTAL, LED_PIN, NEO_GRB + NEO_KHZ800) 
{
}

void LedController::begin() {
    strip.begin();
    strip.show();
    showStartupAnimation();
}

void LedController::clear() {
    strip.clear();
}

void LedController::show() {
    strip.show();
}

void LedController::setPixel(int i, uint32_t c) {
    strip.setPixelColor(i, c);
}

void LedController::setWord(Word w, uint32_t c) {
    for (int i = w.start; i <= w.end; i++) {
        setPixel(i, c);
    }
}

void LedController::setCorner(int minutes, uint32_t c) {
    // 4 corners used for minutes 1, 2, 3, 4
    // Indices 119, 120, 121, 122
    for(int i=0; i<minutes; i++) {
        setPixel(NUM_LEDS_GRID + i, c);
    }
}

uint32_t LedController::getCurrentColor() {
    return _settings->getColor(); 
    // Note: Settings color is usually RGB. 
    // Ideally we ignore brightness component here and let setBrightness handle it,
    // but NeoPixel setBrightness is global and destructive.
    // Better to scale the color manually or use strip.setBrightness() globally.
    // We will use strip.setBrightness() in applySettings().
}

void LedController::showStartupAnimation() {
    for(int i=0; i<=NUM_LEDS_TOTAL; i++) {
        setPixel(i, strip.Color(255, 0, 0));
        strip.show();
        delay(50);
        if (i == 4 || i == 17 || i == 18 || i == 29 || i == 41){
            setPixel(i, strip.Color(255, 255, 0));
        } else if (i == 53 || i == 54 || i == 64 || i == 65 || i == 66 || i == 76) {
            setPixel(i, strip.Color(255, 128, 0));
        } else if (i == 77 || i == 78 || i == 79 || i == 89 || i == 100 || i == 114 || i == 115 ) {
            setPixel(i, strip.Color(0, 255, 0));
        } else {
            setPixel(i, strip.Color(0, 0, 0));
        }
        strip.show();
    }
    strip.show();
    delay(3000);
}

void LedController::applySettings() {
    struct tm t = timeManager.getCurrentTime();
    int currentHour = t.tm_hour;
    
    int br = _settings->getDayBrightness();
    if (_settings->isNightMode(currentHour)) {
        br = _settings->getNightBrightness();
        // Limit minimum brightness to avoid "off"
        if(br < 5 && br > 0) br = 5; 
    }
    
    // Check if we should be OFF
    // Note: getLocalTime returns 0-6 for wday where 0=Sunday
    if (_settings->isOffTime(t.tm_wday, currentHour)) {
        br = 0;
    }

    strip.setBrightness(br);
}

void LedController::loop() {
    // Check every 30 seconds
    if (millis() - lastUpdate > 30000) {
        lastUpdate = millis();
        applySettings();
        
        struct tm t = timeManager.getCurrentTime();
        int min = t.tm_min;

        if (_settings->getShowDate()) {
            if ((min == 0 || min == 30) && (t.tm_sec < 30)) {
                updateDateDisplay();
                show();
                return;
            }
        }
            
        updateTimeDisplay();
        show();
    }
}

void LedController::updateTimeDisplay() {
    clear();
    struct tm t = timeManager.getCurrentTime();
    
    uint32_t color = getCurrentColor();
    
    // Always IT IS
    setWord(Words::DAT, color);
    setWord(Words::IS, color);
    
    int h = t.tm_hour;
    int m = t.tm_min;
    
    int fiveMinBlock = m / 5; 
    int remainder = m % 5;
    
    setCorner(remainder, color);
    
    // Determine words based on fiveMinBlock (0 to 11)
    // 0: KLOCK
    // 1: FIEV NA
    // 2: TEIHN NA
    // 3: KARTEER NA
    // 4: TWINTIG NA
    // 5: FIEV VÖR HALV
    // 6: HALV
    // 7: FIEV NA HALV
    // 8: TWINTIG VÖR
    // 9: KARTEER VÖR
    // 10: TEIHN VÖR
    // 11: FIEV VÖR
    
    // Handle TO logic (hour increment)
    int displayHour = h;
    bool isTo = false;
    
    if (fiveMinBlock > 6) {
        displayHour++;
        isTo = true;
    }
    
    // 12h format
    displayHour = displayHour % 12;
    if (displayHour == 0) displayHour = 12;
    
    // Minutes Words
    switch (fiveMinBlock) {
        case 0:
            setWord(Words::AL, color);
            setWord(Words::KLOCK, color); 
            break;
        case 1: 
            setWord(Words::FIEV_MIN, color); 
            setWord(Words::NA, color); 
            break;
        case 2: 
            setWord(Words::TEIHN_MIN, color); 
            setWord(Words::NA, color); 
            break;
        case 3: 
            setWord(Words::KARTEER, color); 
            setWord(Words::NA, color); 
            break;
        case 4: 
            setWord(Words::TWINTIG, color); 
            setWord(Words::NA, color); 
            break;
        case 5: 
            setWord(Words::FIEV_MIN, color); 
            setWord(Words::VOER, color); 
            setWord(Words::HALV2, color); 
            break;
        case 6: 
            setWord(Words::HALV2, color);
            break;
        case 7: 
            setWord(Words::FIEV_MIN, color); 
            setWord(Words::NA, color); 
            setWord(Words::HALV2, color); 
            break;
        case 8: 
            setWord(Words::TWINTIG, color); 
            setWord(Words::VOER, color); 
            break;
        case 9: 
            setWord(Words::KARTEER, color); 
            setWord(Words::VOER, color); 
            break;
        case 10: 
            setWord(Words::TEIHN_MIN, color); 
            setWord(Words::VOER, color); 
            break;
        case 11: 
            setWord(Words::FIEV_MIN, color); 
            setWord(Words::VOER, color); 
            break;
    }
    
    // Hour Words
    switch (displayHour) {
        case 1: setWord(Words::EEN, color); break;
        case 2: setWord(Words::TWEE, color); break;
        case 3: setWord(Words::DREE, color); break;
        case 4: setWord(Words::VEER, color); break;
        case 5: setWord(Words::FIEV, color); break;
        case 6: setWord(Words::SESS, color); break;
        case 7: setWord(Words::SOEVEN, color); break;
        case 8: setWord(Words::ACHT, color); break;
        case 9: setWord(Words::NEGEN, color); break;
        case 10: setWord(Words::TEIHN, color); break;
        case 11: setWord(Words::ELVEN, color); break;
        case 12: setWord(Words::TWALVEN, color); break;
    }
}

void LedController::updateDateDisplay() {
    clear();
    struct tm t = timeManager.getCurrentTime();
    
    int d = t.tm_mday;
    int m = t.tm_mon + 1; // tm_mon is 0-11
    
    // Safety check for array bounds
    int d_tens = d / 10;
    int d_ones = d % 10;
    int m_tens = m / 10;
    int m_ones = m % 10;
    
    if (d_tens > 9) d_tens = 0; 
    
    matrixDate(upLeft, numbers[d_tens], 0, 255, 0); // Greenish
    matrixDate(upRight, numbers[d_ones], 0, 255, 0);
    matrixDate(downLeft, numbers[m_tens], 255, 128, 0); // Orange-ish
    matrixDate(downRight, numbers[m_ones], 255, 128, 0);
}

void LedController::matrixDate(const int place[], const bool number[], int r, int g, int b) {
    uint32_t color = strip.Color(r, g, b);
    for (int i = 0; i < 25; i++) {
        if (number[i]) { 
             setPixel(place[i], color);
        }
    }
}

void LedController::testAllWords() {
    Word allWords[] = {
        Words::DAT, Words::IS, Words::HALV1, Words::AL,
        Words::KLOCK, Words::KARTEER,
        Words::TWINTIG, Words::FIEV_MIN,
        Words::TEIHN_MIN, Words::VOER, Words::NA,
        Words::HALV2,
        Words::EEN, Words::TWEE, Words::DREE, Words::VEER,
        Words::FIEV, Words::SESS, Words::SOEVEN, Words::ACHT,
        Words::NEGEN, Words::TEIHN, Words::ELVEN, Words::TWALVEN
    };

    int numWords = sizeof(allWords) / sizeof(allWords[0]);

    for (int i = 0; i < numWords; i++) {
        clear();
        uint32_t randomColor = strip.Color(random(0, 255), random(0, 255), random(0, 255));
        setWord(allWords[i], randomColor);
        show();
        delay(1000); 
    }
    clear();
    show();
}

void LedController::testAllDates() {
    for (int i = 0; i < 10; i++) {
        clear();
        matrixDate(upLeft, numbers[i], random(0, 255), random(0, 255), random(0, 255));
        matrixDate(upRight, numbers[i], random(0, 255), random(0, 255), random(0, 255));
        matrixDate(downLeft, numbers[i], random(0, 255), random(0, 255), random(0, 255));
        matrixDate(downRight, numbers[i], random(0, 255), random(0, 255), random(0, 255));
        show();
        delay(2000);
    }
    clear();
    show();
}

void LedController::testAllCorners() {
    uint32_t color = getCurrentColor();
    for (int i = 0; i < 5; i++) {
        setCorner(i, color);
        show();
        delay(1000);
    }
}