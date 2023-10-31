
#ifndef DEVICE_SEND_THREAD_h
#define DEVICE_SEND_THREAD_h

#include <Thread.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ABSESP8266.h>


#include "options.h"


class SendThread : public Thread {

public:
    using Device = esp8266::AbstractDevice;

protected:
    int _qty_unsuccessful_try = 0;
    const int _limit_qty_unsuccessful_try = 10;

protected:
    DeviceOption* _device_option;
    Device* _device = Device::getInstance();

protected:
    esp8266::StatusMode updateState(bool result_request) {
        if (result_request) {
            _device->setStatusMode(esp8266::StatusMode::IN_WORK);
            this->_qty_unsuccessful_try = 0;
            Serial.println("Данные отправлены");
        }
        else {
            this->_qty_unsuccessful_try = this->_qty_unsuccessful_try + 1;
            Serial.println("Не могу отправить данные");
        }

        if (this->_qty_unsuccessful_try >= this->_limit_qty_unsuccessful_try) {
            _device->setStatusMode(esp8266::StatusMode::WAIT_CONNECT);
        }
        return _device->getStatusMode();
    }

public:
    SendThread(DeviceOption* device_option) : Thread(), _device_option(device_option) {
            this->setInterval(1000);
        }

public:
    void run() override {
        this->callback_();
        this->runned();
    }

public:
    String getPayload() {
        StaticJsonDocument<200> doc;
        const auto& id = this->_device->getID();
        doc["ID"] = id;
        doc["data"] = this->_device_option->toJson();
        String result;
        serializeJson(doc, result);
        return result;
    }

    void callback_() {
        if (true /*|| _device_option->readyToSend()*/) {
            const String& payload = this->getPayload();
            Serial.println(payload);
            auto is_success = this->sendData(monitoringSystemServer.getURL(), payload);
            this->updateState(is_success);
        }
        
    }

    bool sendData(const String& server_name, const String& payload) {
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
        Serial.println("Не могу подключится к Wi-Fi сети");
        return false;
    }
};

class MachineСonditionSendThread : public SendThread {

public:
    MachineСonditionSendThread(MachineCondition* mss_option) : SendThread(mss_option) {}
};

#endif