#ifndef WEBINTERFACE_H
#define WEBINTERFACE_H

#include <WebServer.h>
#include "Settings.h"

class WebInterface {
public:
    WebInterface(Settings* settings);
    void begin();
    void handleClient();

private:
    Settings* _settings;
    WebServer server;

    void handleRoot();
    void handleGetSettings();
    void handlePostSettings();
};

#endif
