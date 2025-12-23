#include "TimeManager.h"

// Europe/Berlin as default, but can be changed or made configurable
// "CET-1CEST,M3.5.0,M10.5.0/3" is the POSIX string for Central Europe
const char* NTP_SERVER = "pool.ntp.org";
const char* TZ_INFO = "CET-1CEST,M3.5.0,M10.5.0/3"; 

void TimeManager::begin() {
    configTime(0, 0, NTP_SERVER);
    setenv("TZ", TZ_INFO, 1);
    tzset();
}

void TimeManager::update() {
    // nothing needed for standard time.h, it syncs in background
}

struct tm TimeManager::getCurrentTime() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        // Return clean zero struct if failed
        memset(&timeinfo, 0, sizeof(struct tm));
    }
    return timeinfo;
}

bool TimeManager::isTimeSet() {
    struct tm timeinfo;
    return getLocalTime(&timeinfo);
}

String TimeManager::getFormattedTime() {
    struct tm t = getCurrentTime();
    char b[10];
    snprintf(b, 10, "%02d:%02d:%02d", t.tm_hour, t.tm_min, t.tm_sec);
    return String(b);
}

String TimeManager::getFormattedDate() {
    struct tm t = getCurrentTime();
    char b[20];
    snprintf(b, 20, "%02d.%02d.%04d", t.tm_mday, t.tm_mon + 1, t.tm_year + 1900);
    return String(b);
}
