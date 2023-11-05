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

Thread* thread1 = new MachineConditionSendThread();
Thread* thread2 = new QtyDetailsSendThread();
Thread* thread3 = new TimeWorkSendThread();

void setup() {
    Serial.begin(9600);

    thread_controller.add(thread1); // Контроль состояния станка
    thread_controller.add(thread2); // Счетчик количества деталей
    thread_controller.add(thread3); // Период работы

    esp8266::AbstractDevice::waitConnectToWiFi();
    waitForSync();

	Serial.println("UTC: " + UTC.dateTime());
}

void loop() {
    thread_controller.run();

    esp8266::AbstractDevice::updateMachineCondition(&machineCondition);
    esp8266::AbstractDevice::updateQtyDetails(&qtyDetails);
    esp8266::AbstractDevice::updateTimeWork(&timeWork);
}