#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>
#include <Preferences.h>

struct WordClockConfig {
    String wifi_ssid;
    String wifi_pass;
    uint32_t color_primary;
    int brightness_day;
    int brightness_night;
    int night_start_hour;
    int night_end_hour;
    // Schedule: 7 days, start/end hour for OFF time. 
    // Simplified: "Off Interval" that applies to enabled days.
    // Bitmask for days: 0b01111111 (Sun-Sat)
    uint8_t off_days_mask; 
    int off_start_hour;
    int off_end_hour;
    bool show_date;
};

class Settings {
public:
    Settings();
    void begin();
    
    // Getters
    WordClockConfig getConfig();
    String getSsid();
    String getPass();
    uint32_t getColor();
    int getDayBrightness();
    int getNightBrightness();
    bool isNightMode(int currentHour);
    bool isOffTime(int currentDoW, int currentHour); // DoW: 0=Sun, 1=Mon...
    bool getShowDate();

    // Setters
    void setWifi(String ssid, String pass);
    void setColor(uint32_t color);
    void setBrightness(int day, int night);
    void setNightInterval(int start, int end);
    void setOffSchedule(uint8_t daysMask, int start, int end);
    void setShowDate(bool enabled);

private:
    Preferences prefs;
    WordClockConfig cache;
    void loadFromPrefs();
};

#endif
