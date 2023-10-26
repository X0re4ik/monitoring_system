
#ifndef DEVICE_HTTP_CLIENT_h
#define DEVICE_HTTP_CLIENT_h

#include "options.h"

#include <Thread.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ABSESP8266.h>
#include "options.h"




class SendThread : public Thread {

public:
    using Device = esp8266::AbstractDevice;

protected:

protected:
    int _qty_unsuccessful_try = 0;
    const int _limit_qty_unsuccessful_try = 10;

protected:
    WiFiOptions* _wifi_option;
    MonitoringSystemServer* _monitoring_system_server_option;
    DeviceOption* _device_option;
    Device* _device = Device::getInstance();

protected:
    virtual void callback_() = 0;
    virtual bool sendData(const String& server_name, const String& payload) = 0;

protected:
    esp8266::StatusMode updateState(bool result_request) {
        if (result_request) {
            _device->setStatusMode(esp8266::StatusMode::IN_WORK);
            this->_qty_unsuccessful_try = 0;
        }
        else {
            this->_qty_unsuccessful_try = this->_qty_unsuccessful_try + 1;
        }

        if (this->_qty_unsuccessful_try >= this->_limit_qty_unsuccessful_try) {
            _device->setStatusMode(esp8266::StatusMode::WAIT_CONNECT);
        }
        return _device->getStatusMode();
    }

public:
    SendThread(
        WiFiOptions* wifi_option, 
        MonitoringSystemServer* mss_option,
        DeviceOption* device_option) : Thread(), _monitoring_system_server_option(mss_option), _wifi_option(wifi_option), _device_option(device_option) {
            this->setInterval(1000);
        }

    void run() override {
        this->callback_();
        this->runned();
    }

    String getPayload() {
        DynamicJsonDocument doc(1024);
        const auto& id = this->_device->getID();
        doc["ID"] = id;
        doc["data"] = this->_device_option->toJson();
        String result;
        serializeJson(doc, result);
        return result;
    }
};

class MachineСonditionSendThread : public SendThread {

public:

    MachineСonditionSendThread(
        WiFiOptions* wifi_option, 
        MonitoringSystemServer* mss_option) : SendThread(wifi_option, mss_option, &machine_condition) {}

    void callback_() override {
        if (_device_option->readyToSend()) {
            const String& payload = this->getPayload();
            auto is_success = this->sendData(this->_monitoring_system_server_option->getURL(), payload);
            this->updateState(is_success);
        }
        
    }

    bool sendData(const String& server_name, const String& payload) override {
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
};

#endif