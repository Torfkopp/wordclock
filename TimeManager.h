#ifndef TIMEMANAGER_H
#define TIMEMANAGER_H

#include <Arduino.h>
#include <time.h>

class TimeManager {
public:
    void begin();
    void update();
    struct tm getCurrentTime();
    String getFormattedTime();
    String getFormattedDate();
    bool isTimeSet();
};

#endif
