#include "hardware.hh"
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <softPwm.h>
#include <cassert>

const int RED_PIN = 27;
const int GREEN_PIN = 28;
const int BLUE_PIN = 29;
const int PRESETBTN_PIN = 16;
const int MIXBTN_PIN = 15;
const int DEBOUNCE_TIME = 50;

//************************************************************************
//**********************    Buttons component ****************************
//************************************************************************

Buttons::Buttons() {
  wiringPiSetup();
  pinMode(PRESETBTN_PIN, INPUT);
  pinMode(MIXBTN_PIN, INPUT);

  pullUpDnControl(PRESETBTN_PIN, PUD_UP);
  pullUpDnControl(MIXBTN_PIN, PUD_UP);

  this->preset_btn = false;
  this->last_preset_btn = false;
  this->mix_btn = false;
  this->last_mix_btn = false;
  this->last_dbnc_time = 0;
}

bool Buttons::pollButtons() {
  bool stateChanged = false;
  int new_preset_btn = !digitalRead(PRESETBTN_PIN);
  int new_mix_btn = !digitalRead(MIXBTN_PIN);

  if (new_preset_btn != this->last_preset_btn) {
    this->last_dbnc_time = millis();
  }

  if (new_mix_btn != this->last_mix_btn) {
    this->last_dbnc_time = millis();
  }

  if((millis() - last_dbnc_time) > DEBOUNCE_TIME) {

    if(new_preset_btn != this->preset_btn) {
      this->preset_btn = new_preset_btn;
      stateChanged = true;
    }

    if(new_mix_btn != this->mix_btn) {
      this->mix_btn = new_mix_btn;
      stateChanged = true;
    }
  }
  this->last_preset_btn = new_preset_btn;
  this->last_mix_btn = new_mix_btn;
  return stateChanged;
}

//************************************************************************
//**********************    RGB LED component ****************************
//************************************************************************

RgbLed::RgbLed() {
  wiringPiSetup();
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  softPwmCreate(RED_PIN, 0, 100); // Limited to 100
  softPwmCreate(GREEN_PIN, 0, 100); // see https://projects.drogon.net/raspberry-pi/wiringpi/software-pwm-library/
  softPwmCreate(BLUE_PIN, 0, 100);
}

void RgbLed::SetRed(int value) {
  softPwmWrite(RED_PIN, value);
}

void RgbLed::SetGreen(int value) {
  softPwmWrite(GREEN_PIN, value);
}

void RgbLed::SetBlue(int value) {
  softPwmWrite(BLUE_PIN, value);
}

void RgbLed::SetColour(Colour colour) {
  softPwmWrite(RED_PIN, colour.red);
  softPwmWrite(GREEN_PIN, colour.green);
  softPwmWrite(BLUE_PIN, colour.blue);
}

void RgbLed::Dim() {
  softPwmWrite(RED_PIN, 0);
  softPwmWrite(GREEN_PIN, 0);
  softPwmWrite(BLUE_PIN, 0);
}

//************************************************************************
//**********************    PotMeter component ***************************
//************************************************************************

PotMeter::PotMeter() {
  int spi_fd = wiringPiSPISetup(0, 1000000);
  assert(spi_fd != -1 && "Could not start wiringPi SPI");

  this->minValue = 20;
  this->maxValue = 1023;
}

int PotMeter::read() {
  unsigned char data[3];
  data[0] = 1;
  data[1] = 1<<7;
  data[2] = 0;

  static_assert(1 == sizeof(unsigned char), "Unsigned char is bigger than 1 on platform");
  wiringPiSPIDataRW(0, data, 3);
  return (((data[1] << 8) & 11<<8) | (data[2] & 0xff));
}

int PotMeter::map(int value, int min_out, int max_out) {
  return (value - this->minValue) * (max_out - min_out) / (this->maxValue - this->minValue) + min_out;
}
