
#ifndef DEVICE_HTTP_CLIENT_h
#define DEVICE_HTTP_CLIENT_h

#include "options.h"

#include <Thread.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ABSESP8266.h>

class DeviceHTTPClient {
public:
    DeviceHTTPClient(WiFiOptions* wifi_options) : DeviceHTTPClient(wifi_options->wifi_name, wifi_options->password) {}

    DeviceHTTPClient(String wifi_name, String wifi_password) : _wifi_name(wifi_name), _wifi_password(wifi_password) {
        
    }

    bool send(String server_name, String payload) {
        if(WiFi.status() == WL_CONNECTED){
            WiFiClient client;
            HTTPClient http;

            http.begin(client, server_name);
            http.addHeader("Connection", "Keep-Alive");
            http.addHeader("Content-Type", "application/json");

            int http_response_code = http.POST(payload);
            http.end();
                
            return http_response_code == 200;
        }
        return false;
    }

private:

    String _wifi_name;
    String _wifi_password;
};


class DeviceHTTPClientThread : public Thread {

private:
    int _qty_unsuccessful_try = 0;
    const int _limit_qty_unsuccessful_try = 10;

private:
    WiFiOptions* _wifi_option;
    MonitoringSystemServer* _monitoring_system_server_option;


public:

    DeviceHTTPClientThread(
        WiFiOptions* wifi_option, 
        MonitoringSystemServer* monitoring_system_server_option, 
        unsigned long interval = 1000) : Thread(), _monitoring_system_server_option(monitoring_system_server_option), _wifi_option(wifi_option) {
        this->setInterval(interval);
    }

    void run() override {
        this->callback();
        this->runned();
    }

    void callback() {
        auto http_client = DeviceHTTPClient(this->_wifi_option);

        auto is_success = http_client.send(this->_monitoring_system_server_option->getURL(), "Hello, I am esp8266");

        if (is_success) {
            esp8266::AbstractDevice::getInstance()->setStatusMode(esp8266::StatusMode::IN_WORK);
            this->_qty_unsuccessful_try = 0;
        }
        else {
            this->_qty_unsuccessful_try = this->_qty_unsuccessful_try + 1;
        }


        if (this->_qty_unsuccessful_try >= this->_limit_qty_unsuccessful_try) {
            esp8266::AbstractDevice::getInstance()->setStatusMode(esp8266::StatusMode::WAIT_CONNECT);
            Serial.println("Слишком много неудачных попыток");
        }
    }
};



#endif