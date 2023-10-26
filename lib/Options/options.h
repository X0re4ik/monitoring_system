#ifndef OPTIONS_h
#define OPTIONS_h

#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ezTime.h>

static struct MonitoringSystemServer {

    String getURL() {
        return "http://192.168.0.113:5000/";
    }

} monitoringSystemServer;


static struct WiFiOptions {
    String wifi_name = "GoldSl";
    String password = "sl19701974sl";
} wifiOptions;


struct DeviceOption {
    virtual String toJson();
    virtual void reset();
    virtual bool readyToSend();
};


static struct MachineCondition : public DeviceOption {

    float temperature = 0;
    float vibration = 0;
    float spindle_speed = 0;
    Timezone* dispatch_time = nullptr;

    String toJson() override {
        DynamicJsonDocument doc(1024);

        doc["temperature"]   = this->temperature;
        doc["vibration"]     = this->vibration;
        doc["spindle_speed"] = this->spindle_speed;
        //doc["dispatch_time"] = this->dispatch_time->dateTime();

        String result;
        serializeJson(doc, result);
        return result;
    }


    void reset() override {
        this->dispatch_time = nullptr;
    }

    bool readyToSend() override {
        return true || dispatch_time != nullptr;
    }

} machine_condition;

static struct QtyDetails : public DeviceOption {

    size_t qty_good_details;
    size_t qty_bad_details;
    Timezone* dispatch_time = nullptr;

    String toJson() {
        DynamicJsonDocument doc(1024);

        doc["qty_good_details"]   = this->qty_good_details;
        doc["qty_bad_details"]     = this->qty_bad_details;
        //doc["dispatch_time"] = this->dispatch_time->dateTime();

        String result;
        serializeJson(doc, result);
        return result;
    }

    void reset() {
        this->dispatch_time = nullptr;
    }

    bool readyToSend() {
        return dispatch_time != nullptr;
    }

} qty_details;

static struct MachinePerformance : public DeviceOption {

    Timezone* time_start_work = nullptr;
    Timezone* time_end_work = nullptr;

    String toJson() {
        DynamicJsonDocument doc(1024);

        doc["time_start_work"]   = this->time_start_work->dateTime();
        doc["time_end_work"]     = this->time_end_work->dateTime();

        String result;
        serializeJson(doc, result);
        return result;
    }

    void reset() {
        this->time_start_work = nullptr;
        this->time_end_work = nullptr;
    }

    bool readyToSend() {
        return (time_start_work != nullptr) && (time_end_work != nullptr);
    }

} machine_performance;

#endif