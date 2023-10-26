#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager

#include <ABSESP8266.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>


#include "DeviceHTTPClient.h"

#include <ThreadController.h>

void sendDataAboutState() {
    Serial.println(wifiOptions.wifi_name);
}

ThreadController thread_controller = ThreadController();


Thread* thread1 = new MachineСonditionSendThread(&wifiOptions, &monitoringSystemServer);
Thread* thread2 = new Thread(sendDataAboutState, 1000);



void setup() {

    Serial.begin(9600);

	thread_controller.add(thread1);
	thread_controller.add(thread2);
    WiFi.begin(wifiOptions.wifi_name, wifiOptions.password);
}

void loop() {
    thread_controller.run();
}
