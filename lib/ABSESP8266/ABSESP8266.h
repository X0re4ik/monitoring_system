

#ifndef ESP8266_ABS_DEVICE_h
#define ESP8266_ABS_DEVICE_h

#include <ESP8266WiFi.h>
#include "options.h"
#include <WiFiManager.h>
#include <ezTime.h>

namespace esp8266 {
    
enum StatusMode {
    IN_WORK = 0,
    WAIT_CONNECT = 1,
    ERROR = 2
};

enum DevicePins {
    START_WORK = 1,
    END_WORK = 2,
    INCREASE_QTY_GOOD_DETAILS = 3,
    DECREASE_QTY_GOOD_DETAILS = 4,
    TEMPERATURE = 0,
    VIBRATION = 1,
    SPINDLE_SPEED = 2
};


int _digitalRead(const DevicePins& pin) {
    return 1; //digitalRead(pin);
}



class AbstractDevice {

public:

    static AbstractDevice* getInstance() {
        static AbstractDevice instance;
        return &instance;
    }

    String getID() {
        return this->getMACAddress();
    }

    String getMACAddress() {
        return this->_mac_address;
    }

    void setWiFiMode(bool mode) {
        this->_connected_to_wifi = false;
    }

    bool getWiFiMode() {
        return this->_connected_to_wifi;
    }

    void setStatusMode(StatusMode status_mode) {
        this->_status_mode = status_mode;
    }

    StatusMode getStatusMode() {
        digitalWrite(this->_status_mode, HIGH);
        return this->_status_mode;
    }

private:

    AbstractDevice() {
        this->_mac_address = WiFi.macAddress();
        this->_connected_to_wifi = false;
        this->_status_mode = StatusMode::WAIT_CONNECT;

        // pinMode(DevicePins::START_WORK, OUTPUT);
        // pinMode(DevicePins::END_WORK, OUTPUT);
        // pinMode(DevicePins::INCREASE_QTY_GOOD_DETAILS, OUTPUT);
        // pinMode(DevicePins::DECREASE_QTY_GOOD_DETAILS, OUTPUT);
        pinMode(DevicePins::TEMPERATURE, INPUT);
        pinMode(DevicePins::VIBRATION, INPUT);
        pinMode(DevicePins::SPINDLE_SPEED, INPUT);

    }

public:

    static void updateMachineCondition(MachineCondition* machine_condition) {
        machine_condition->spindle_speed = digitalRead(DevicePins::SPINDLE_SPEED);
        machine_condition->temperature = digitalRead(DevicePins::TEMPERATURE);
        machine_condition->vibration = digitalRead(DevicePins::VIBRATION);
        machine_condition->dispatch_time = UTC.dateTime(ISO8601);
    }

    static void updateQtyDetails(QtyDetails* qty_details) {
        int increase_qty_good_details_button = _digitalRead(DevicePins::INCREASE_QTY_GOOD_DETAILS);
        int decrease_qty_good_details_button = _digitalRead(DevicePins::DECREASE_QTY_GOOD_DETAILS);

        if (increase_qty_good_details_button == HIGH) {
            qty_details->qty_good_details += 1;
        }
        if (decrease_qty_good_details_button == HIGH) {
            qty_details->qty_bad_details += 1;
        }
        qty_details->dispatch_time = UTC.dateTime(ISO8601);
    }

    static void updateTimeWork(TimeWork* time_work) {
        int start_work = _digitalRead(DevicePins::START_WORK);
        int end_work = _digitalRead(DevicePins::END_WORK);

        if (start_work == HIGH) {
            time_work->start = UTC.dateTime(ISO8601);
        }
        if (end_work == HIGH) {
            time_work->end = UTC.dateTime(ISO8601);
        }
    }


    static void activateAccessPoint(WiFiOptions* settings) {
        Serial.println("Активирую точку доступа");
        WiFiManager wifiManager;
        //wifiManager.resetSettings();
        wifiManager.autoConnect("AutoConnectAP");
        

        String name = wifiManager.getWiFiSSID(false);
        String pass = wifiManager.getWiFiPass(false);
        settings->name = name;
        settings->password = pass;
    }

    static void waitConnectToWiFi() {
        Serial.println("Не могу соединиться с WiFi");
        AbstractDevice::activateAccessPoint(&wifiOptions);
        WiFi.begin(wifiOptions.name, wifiOptions.password);
        Serial.println("Wi-Fi: " + wifiOptions.name + " [" + wifiOptions.password +"]");
        Serial.println("Соединение успешно завершено");
    }

private:
    String _mac_address;
    bool _connected_to_wifi;
    StatusMode _status_mode;

};
}

#endif