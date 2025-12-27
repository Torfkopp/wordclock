#include "Settings.h"

Settings::Settings() {}

void Settings::begin() {
    prefs.begin("wordclock", false);
    loadFromPrefs();
}

void Settings::loadFromPrefs() {
    cache.wifi_ssid = prefs.getString("ssid", "");
    cache.wifi_pass = prefs.getString("pass", "");
    cache.color_primary = prefs.getUInt("color", 0x00FFAA00); // Default Orange-ish
    cache.brightness_day = prefs.getInt("br_day", 42);
    cache.brightness_night = prefs.getInt("br_night", 21);
    cache.night_start_hour = prefs.getInt("n_start", 22);
    cache.night_end_hour = prefs.getInt("n_end", 7);
    cache.off_days_mask = prefs.getUChar("off_days", 0); // Default none
    cache.off_start_hour = prefs.getInt("o_start", 0);
    cache.off_end_hour = prefs.getInt("o_end", 0);
    cache.show_date = prefs.getBool("show_date", true);
}

WordClockConfig Settings::getConfig() {
    return cache;
}

String Settings::getSsid() { return cache.wifi_ssid; }
String Settings::getPass() { return cache.wifi_pass; }
uint32_t Settings::getColor() { return cache.color_primary; }
int Settings::getDayBrightness() { return cache.brightness_day; }
int Settings::getNightBrightness() { return cache.brightness_night; }
bool Settings::getShowDate() { return cache.show_date; }

bool Settings::isNightMode(int currentHour) {
    if (cache.night_start_hour == cache.night_end_hour) return false;
    
    if (cache.night_start_hour < cache.night_end_hour) {
        // e.g. 09:00 to 17:00
        return (currentHour >= cache.night_start_hour && currentHour < cache.night_end_hour);
    } else {
        // e.g. 22:00 to 07:00 (Overnight)
        return (currentHour >= cache.night_start_hour || currentHour < cache.night_end_hour);
    }
}

bool Settings::isOffTime(int currentDoW, int currentHour) {
    // Formula to change to normal weekday format instead of the weird US-American one
    currentDoW = (currentDoW - 1) % 7; // currentDoW: 0=Mon...5=Sat, 6=Sun
    // Check if today is an "off day"
    if (!((cache.off_days_mask >> currentDoW) & 1)) {
        return false; // Today is not enabled for shutting off
    }

    if (cache.off_start_hour == cache.off_end_hour) return false;

    if (cache.off_start_hour < cache.off_end_hour) {
        return (currentHour >= cache.off_start_hour && currentHour < cache.off_end_hour);
    } else {
        return (currentHour >= cache.off_start_hour || currentHour < cache.off_end_hour);
    }
}

void Settings::setWifi(String ssid, String pass) {
    cache.wifi_ssid = ssid;
    cache.wifi_pass = pass;
    prefs.putString("ssid", ssid);
    prefs.putString("pass", pass);
}

void Settings::setColor(uint32_t color) {
    cache.color_primary = color;
    prefs.putUInt("color", color);
}

void Settings::setBrightness(int day, int night) {
    cache.brightness_day = day;
    cache.brightness_night = night;
    prefs.putInt("br_day", day);
    prefs.putInt("br_night", night);
}

void Settings::setNightInterval(int start, int end) {
    cache.night_start_hour = start;
    cache.night_end_hour = end;
    prefs.putInt("n_start", start);
    prefs.putInt("n_end", end);
}

void Settings::setOffSchedule(uint8_t daysMask, int start, int end) {
    cache.off_days_mask = daysMask;
    cache.off_start_hour = start;
    cache.off_end_hour = end;
    prefs.putUChar("off_days", daysMask);
    prefs.putInt("o_start", start);
    prefs.putInt("o_end", end);
}

void Settings::setShowDate(bool enabled) {
    cache.show_date = enabled;
    prefs.putBool("show_date", enabled);
}
