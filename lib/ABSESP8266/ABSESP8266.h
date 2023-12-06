

#ifndef ESP8266_ABS_DEVICE_h
#define ESP8266_ABS_DEVICE_h

#include <ESP8266WiFi.h>
#include "options.h"
#include <WiFiManager.h>
#include <ezTime.h>

#include "DHT.h"
#include <SimpleDHT.h>
#include <Button.h>



namespace esp8266 {
    
enum StatusMode {
    IN_WORK = 6,
    WAIT_CONNECT = 5,
    ERROR = 4
};

enum DevicePins {
    START_WORK = 12,
    END_WORK = 14,
    INCREASE_QTY_GOOD_DETAILS = 15,
    INCREASE_QTY_BAD_DETAILS = 13,
    TEMPERATURE = 0,
    VIBRATION = 1,
    SPINDLE_SPEED = 2
};





Button increase_qty_good_details_button = Button(DevicePins::INCREASE_QTY_GOOD_DETAILS);
Button increase_qty_bad_details_button = Button(DevicePins::INCREASE_QTY_BAD_DETAILS);

Button start_work_button = Button(DevicePins::START_WORK);
Button end_work_button = Button(DevicePins::END_WORK);




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

        pinMode(StatusMode::IN_WORK, OUTPUT);
        pinMode(StatusMode::ERROR, OUTPUT);
        pinMode(StatusMode::WAIT_CONNECT, OUTPUT);
        
        pinMode(DevicePins::START_WORK, INPUT);
        pinMode(DevicePins::END_WORK, INPUT);
        pinMode(DevicePins::INCREASE_QTY_GOOD_DETAILS, INPUT);
        pinMode(DevicePins::INCREASE_QTY_BAD_DETAILS, INPUT);
        pinMode(DevicePins::TEMPERATURE, INPUT);
        pinMode(DevicePins::VIBRATION, INPUT);
        pinMode(DevicePins::SPINDLE_SPEED, INPUT);

    }

public:

    static void updateMachineCondition(MachineCondition* machine_condition, DHT* dht) {
        machine_condition->temperature = random(20, 30);//dht->readTemperature();
        machine_condition->humidity = random(94, 110);//dht->readHumidity();
        machine_condition->pressure = random(15, 33);
        machine_condition->dispatch_time = UTC.dateTime(ISO8601);
    }

    static void updateQtyDetails(QtyDetails* qty_details) {
        qty_details->qty_good_details += increase_qty_good_details_button.read();
        qty_details->qty_bad_details += increase_qty_bad_details_button.read();

        qty_details->dispatch_time = UTC.dateTime(ISO8601);
    }

    static void updateTimeWork(TimeWork* time_work) {
        if (start_work_button.read()) {
            time_work->start = UTC.dateTime(ISO8601);
        }
        if (end_work_button.read()) {
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
        Serial.println("Соединение успешно завершено. Я готов передавать данные на сервер.");
    }

private:
    String _mac_address;
    bool _connected_to_wifi;
    StatusMode _status_mode;

};
}

#endif