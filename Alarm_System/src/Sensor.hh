#ifndef SENSOR_HH
#define SENSOR_HH

#include "skel_Sensor.hh"
#include <dzn/pump.hh>

class Sensor : public skel::Sensor {
private:
  dzn::pump& pump;
  bool sensor_value;
  bool last_sensor_value;
  unsigned long last_dbnc_time;
  bool polling;

public:
  Sensor(const dzn::locator& loc);
  void iSensor_turnOn();
  void iSensor_turnOff();
  void poll();
};

#endif // SENSOR_HH
