#ifndef OPTIONS_h
#define OPTIONS_h

#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

static struct MonitoringSystemServer {

    String origin = "http://192.168.0.113:8000/api/v1/device/";

    String getURL() {
        return  this->origin;
    }


    String getURL(String path) {
        return this->getURL() + path;
    }

} monitoringSystemServer;


static struct WiFiOptions {
    String name = "";
    String password = "";
} wifiOptions;


struct DeviceOption {

    MonitoringSystemServer* mss = &monitoringSystemServer;

    virtual void toJson(JsonObject*);
    virtual void toJson(DynamicJsonDocument*);
    virtual bool isValid(); 
    virtual void reset();

    virtual String getURL() {
        return mss->getURL();
    }
};


static struct MachineCondition : public DeviceOption {

    float temperature = 0;
    float vibration = 0;
    float spindle_speed = 0;
    String dispatch_time = "";

    void toJson(JsonObject* json) override {
        (*json)["temperature"] = this->temperature;
        (*json)["pressure"] = this->vibration;
        (*json)["humidity"] = this->spindle_speed;
    }

    void toJson(DynamicJsonDocument* doc) {
        JsonObject _json = doc->createNestedObject("values");
        this->toJson(&_json);
        (*doc)["dispatch_time"] = this->dispatch_time;
    }

    bool isValid() override {
        return this->dispatch_time != "";
    }

    void reset() override {
        this->temperature = 0;
        this->vibration = 0;
        this->spindle_speed = 0;
        this->dispatch_time = "";
    }

    String getURL() override {
        return DeviceOption::getURL() + "measurement/create";
    }
} machineCondition;

static struct QtyDetails : public DeviceOption {

    int qty_good_details = 0;
    int qty_bad_details = 0;
    String dispatch_time = "";


    void toJson(JsonObject* json) override {
        (*json)["qty_good_details"] = this->qty_good_details;
        (*json)["qty_bad_details"] = this->qty_bad_details;
        (*json)["dispatch_time"] = this->dispatch_time;
    }

    void toJson(DynamicJsonDocument* doc) {
        (*doc)["qty_good_details"] = this->qty_good_details;
        (*doc)["qty_bad_details"] = this->qty_bad_details;
        (*doc)["dispatch_time"] = this->dispatch_time;
    }

    bool isValid() override {
        int qty_details = (this->qty_good_details+this->qty_bad_details);
        return this->dispatch_time != "" && qty_details != 0;
    }

    void reset() override {
        this->qty_good_details = 0;
        this->qty_bad_details = 0;
        this->dispatch_time = "";
    }

    String getURL() override {
        return DeviceOption::getURL() + "made_detail/create";
    }

} qtyDetails;


static struct TimeWork : public DeviceOption {

    String start = "";
    String end = "";

    void toJson(JsonObject* json) override {
        (*json)["start_work"] = this->start;
        (*json)["end_work"] = this->end;
    }

    void toJson(DynamicJsonDocument* doc) {
        (*doc)["start_work"] = this->start;
        (*doc)["end_work"] = this->end;
    }

    bool isValid() override {
        return this->start != "" && this->end != "";
    }

    void reset() override {
        this->start = "";
        this->end = "";
    }

    String getURL() override {
        return DeviceOption::getURL() + "time_work/create";
    }

} timeWork;

#endif