#include <dzn/locator.hh>
#include <dzn/runtime.hh>

#include <wiringPi.h>
#include "Sensor.hh"

const int DEBOUNCE_TIME = 50;
const int PIN_SENSOR = 16;

Sensor::Sensor(const dzn::locator& loc) : skel::Sensor(loc), pump(loc.get<dzn::pump>()) {
  wiringPiSetup();
  pinMode(PIN_SENSOR, INPUT);

  pullUpDnControl(PIN_SENSOR, PUD_UP);
  this->sensor_value = false;
  this->last_sensor_value = false;
  this->last_dbnc_time = 0;
  this->polling = false;
}

void Sensor::iSensor_turnOn() {
  this->polling = true;
  this->pump( [&] { this->poll(); } );
}

void Sensor::iSensor_turnOff() {
  this->polling = false;
}

void Sensor::poll() {
  int new_sensor_value = !digitalRead(PIN_SENSOR);

  if (new_sensor_value != this->last_sensor_value) {
    this->last_dbnc_time = millis();
  }

  if((millis() - last_dbnc_time) > DEBOUNCE_TIME) {

    if(new_sensor_value != this->sensor_value) {
      this->sensor_value = new_sensor_value;
      if(this->sensor_value) {
        this->pump( [&] { this->iSensor.out.triggered(); } );
      }
    }
  }
  this->last_sensor_value = new_sensor_value;
  if(this->polling) this->pump( [&] { this->poll(); } );
}
