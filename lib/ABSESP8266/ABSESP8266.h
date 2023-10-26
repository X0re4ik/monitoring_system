

#ifndef ESP8266_ABS_DEVICE_h
#define ESP8266_ABS_DEVICE_h

#include <ESP8266WiFi.h>
#include "options.h"

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
    DECREASE_QTY_GOOD_DETAILS = 4
};


int _digitalRead(const DevicePins& pin) {
    return digitalRead(pin);
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
    }

public:

    static void updateMachineCondition(MachineCondition* machine_condition) {
        machine_condition->spindle_speed = 1;
        machine_condition->temperature = 1;
        machine_condition->vibration = 1;
        machine_condition->dispatch_time = new Timezone();
    }

    static void updateMachinePerformance(MachinePerformance* machine_performance) {
        int start_work_button = _digitalRead(DevicePins::START_WORK);
        int end_work_button = _digitalRead(DevicePins::END_WORK);

        if (start_work_button == HIGH) {
            machine_performance->time_start_work = new Timezone();
        }
        else if (end_work_button == HIGH) {
            machine_performance->time_end_work = new Timezone();
        }
    }

    static void updateQtyDetails(QtyDetails* qty_details) {
        int increase_qty_good_details_button = _digitalRead(DevicePins::INCREASE_QTY_GOOD_DETAILS);
        int decrease_qty_good_details_button = _digitalRead(DevicePins::DECREASE_QTY_GOOD_DETAILS);

        if (increase_qty_good_details_button == HIGH) {
            qty_details->qty_good_details++;
        }
        else if (decrease_qty_good_details_button == HIGH) {
            qty_details->qty_bad_details++;
        }
        qty_details->dispatch_time = new Timezone();
    }

private:
    String _mac_address;
    bool _connected_to_wifi;
    StatusMode _status_mode;

};
}

#endif