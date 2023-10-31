#include <ESP8266WiFi.h>          
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <ABSESP8266.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ThreadController.h>

#include "SendThread.h"

ThreadController thread_controller = ThreadController();


Thread* thread1 = new MachineСonditionSendThread(&machine_condition);


void setup() {
    Serial.begin(9600);
    thread_controller.add(thread1);
    esp8266::AbstractDevice::waitConnectToWiFi();
}

void loop() {
    thread_controller.run();
    esp8266::AbstractDevice::updateMachineCondition(&machine_condition);
    delay(1000);
}