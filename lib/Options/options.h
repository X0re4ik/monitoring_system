#ifndef OPTIONS_h
#define OPTIONS_h

#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

static struct MonitoringSystemServer {

    String getURL() {
        return "http://192.168.0.113:5000/";
    }

} monitoringSystemServer;


static struct WiFiOptions {
    String name = "";
    String password = "";
} wifiOptions;


struct DeviceOption {
    virtual JsonObject toJson();
};


static struct MachineCondition : public DeviceOption {

    float temperature = 0;
    float vibration = 0;
    float spindle_speed = 0;
    String dispatch_time = "";

    JsonObject toJson() override {
        StaticJsonDocument<200> doc;

        JsonObject _json = doc.createNestedObject();
        _json["temperature"] = this->temperature;
        _json["vibration"] = this->temperature;
        _json["spindle_speed"] = this->temperature;
        _json["dispatch_time"] = this->dispatch_time;
        return _json;
    }

} machine_condition;

static struct QtyDetails : public DeviceOption {

    size_t qty_good_details = 0;
    size_t qty_bad_details = 0;
    String dispatch_time = "";

    JsonObject toJson() {
        StaticJsonDocument<200> doc;
        
        JsonObject _json = doc.createNestedObject();
        _json["qty_bad_details"] = this->qty_good_details;
        _json["qty_good_details"] = this->qty_bad_details;
        _json["dispatch_time"] = this->dispatch_time;
        return _json;
    }

} qty_details;

#endif