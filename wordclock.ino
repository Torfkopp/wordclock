#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>

#include "Settings.h"
#include "TimeManager.h"
#include "WebInterface.h"
#include "LedController.h"

// ============================================
// CONFIGURATION
// Fill in your WiFi details here for first run
// ============================================
const char* DEFAULT_SSID = "";
const char* DEFAULT_PASS = "";

// Global Objects
Settings settings;
TimeManager timeManager;
WebInterface webInterface(&settings);
LedController ledController(&settings);

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("Starting WordClock...");

    // 1. Initialize Settings
    settings.begin();
    
    // Check if we have stored WiFi, else use default from code
    String ssid = settings.getSsid();
    String pass = settings.getPass();
    
    if (ssid == "") {
        ssid = DEFAULT_SSID;
        pass = DEFAULT_PASS;
        // Optionally save these as defaults
        if (ssid != "YOUR_WIFI_SSID") {
             settings.setWifi(ssid, pass);
        }
    }

    // 2. Connect to WiFi
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), pass.c_str());
    
    Serial.print("Connecting to WiFi");
    int retry = 0;
    while (WiFi.status() != WL_CONNECTED && retry < 20) {
        delay(500);
        Serial.print(".");
        retry++;
    }
    Serial.println();
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.print("Connected! IP: ");
        Serial.println(WiFi.localIP());
        if (MDNS.begin("wordclock")) {
            Serial.println("mDNS responder started");
        }
    } else {
        Serial.println("WiFi Connection Failed! check credentials.");
    }

    // 3. Sync Time
    Serial.print("Syncing Time...");
    timeManager.begin();
    // Blocking wait for time? Not strictly necessary but good for startup
    // We'll just let it sync in background, but maybe wait up to 2s
    delay(2000); 
    if (timeManager.isTimeSet()) {
        Serial.println(" Time Set!");
        Serial.println(timeManager.getFormattedTime());
    } else {
        Serial.println(" Time not yet set (will sync in background)");
    }

    // 4. Start Web Server
    webInterface.begin();

    // 5. Start LEDs
    ledController.begin();
    Serial.println("System Ready.");
}

void loop() {
    // Handle Web Requests
    webInterface.handleClient();
    
    // Maintain Time (internally handled by configTime but good to have a tick if needed)
    timeManager.update();
    
    // Update LEDs
    ledController.loop();
    
    // Reconnect WiFi if lost
    static unsigned long lastWifiCheck = 0;
    if (millis() - lastWifiCheck > 30000) {
        lastWifiCheck = millis();
        if (WiFi.status() != WL_CONNECTED) {
            Serial.println("WiFi lost. Attempting to reconnect...");
            WiFi.disconnect();
            WiFi.reconnect();
        }
    }
}
