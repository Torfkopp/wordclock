#include "WebInterface.h"
#include "index_html.h"

WebInterface::WebInterface(Settings* settings) : _settings(settings), server(80) {}

void WebInterface::begin() {
    server.on("/", HTTP_GET, std::bind(&WebInterface::handleRoot, this));
    server.on("/api/settings", HTTP_GET, std::bind(&WebInterface::handleGetSettings, this));
    server.on("/api/settings", HTTP_POST, std::bind(&WebInterface::handlePostSettings, this));
    server.begin();
}

void WebInterface::handleClient() {
    server.handleClient();
}

void WebInterface::handleRoot() {
    server.send(200, "text/html", INDEX_HTML);
}

void WebInterface::handleGetSettings() {
    WordClockConfig c = _settings->getConfig();
    // Manual JSON construction
    String json = "{";
    json += "\"ssid\":\"" + c.wifi_ssid + "\",";
    json += "\"br_day\":" + String(c.brightness_day) + ",";
    json += "\"br_night\":" + String(c.brightness_night) + ",";
    json += "\"color\":" + String(c.color_primary) + ","; // Send as int
    json += "\"n_start\":" + String(c.night_start_hour) + ",";
    json += "\"n_end\":" + String(c.night_end_hour) + ",";
    json += "\"off_days\":" + String(c.off_days_mask) + ",";
    json += "\"o_start\":" + String(c.off_start_hour) + ",";
    json += "\"o_end\":" + String(c.off_end_hour) + ",";
    json += "\"show_date\":" + String(c.show_date ? "true" : "false");
    json += "}";
    server.send(200, "application/json", json);
}

void WebInterface::handlePostSettings() {
    if (server.hasArg("br_day")) {
        int d = server.arg("br_day").toInt();
        int n = server.arg("br_night").toInt();
        _settings->setBrightness(d, n);
    }
    
    if (server.hasArg("color")) {
        uint32_t c = server.arg("color").toInt();
        _settings->setColor(c);
    }
    
    if (server.hasArg("n_start")) {
        _settings->setNightInterval(server.arg("n_start").toInt(), server.arg("n_end").toInt());
    }
    
    if (server.hasArg("o_start")) {
        _settings->setOffSchedule(server.arg("off_days").toInt(), server.arg("o_start").toInt(), server.arg("o_end").toInt());
    }

    if (server.hasArg("show_date")) {
        _settings->setShowDate(server.arg("show_date").toInt() == 1);
    }

    server.send(200, "text/plain", "OK");
}
