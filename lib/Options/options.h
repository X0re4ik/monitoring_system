#ifndef OPTIONS_h
#define OPTIONS_h

#include <ESP8266WiFi.h>

static struct MonitoringSystemServer {

    String getURL() {
        return "http://192.168.0.113:5000/";
    }

} monitoringSystemServer;


static struct WiFiOptions {
    String wifi_name = "GoldSl";
    String password = "sl19701974sl";
} wifiOptions;

#endif