

#ifndef ESP8266_ABS_DEVICE_h
#define ESP8266_ABS_DEVICE_h

#include <ESP8266WiFi.h>

namespace esp8266 {
    
enum StatusMode {
    IN_WORK = 0,
    WAIT_CONNECT = 1,
    ERROR = 2
};

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
        this->_resetStatusModePins();
        digitalWrite(this->_status_mode, HIGH);
        return this->_status_mode;
    }

private:

    AbstractDevice() {
        this->_mac_address = WiFi.macAddress();
        this->_connected_to_wifi = false;
        this->_status_mode = StatusMode::WAIT_CONNECT;
        
        this->_setStatusModePins();
    }

private:

    void _resetStatusModePins() {
        digitalWrite(StatusMode::IN_WORK, LOW);
        digitalWrite(StatusMode::WAIT_CONNECT, LOW);
        digitalWrite(StatusMode::ERROR, LOW);
    }

    void _setStatusModePins() {
        pinMode(StatusMode::IN_WORK, OUTPUT);
        pinMode(StatusMode::WAIT_CONNECT, OUTPUT);
        pinMode(StatusMode::ERROR, OUTPUT);
    }

private:
    String _mac_address;
    bool _connected_to_wifi;
    StatusMode _status_mode;

};
}

#endif