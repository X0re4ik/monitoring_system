
#ifndef BUTTON_H
#define BUTTON_H

#include <ESP8266WiFi.h>


struct Button {

    Button(const int _button_pin) : button_pin(_button_pin) {}

    const int button_pin;
    int button_push_counter = 0;
    int button_state = 0;
    int last_button_state = 0;

    int read() {
        this->button_state = this->_digitalRead();
        if (this->button_state != this->last_button_state) {
            if (this->button_state == HIGH) {
                this->button_push_counter++;
                Serial.println("on");
            }

            delay(50);
        }
        this->last_button_state = this->button_state;
        return this->button_state;
    }

    int _digitalRead() {
        return digitalRead(this->button_pin);
    }
};
#endif