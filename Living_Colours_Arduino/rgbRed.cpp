#include "Arduino.h"

void setup() {
  pinMode(5, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  analogWrite(5, 128);
  analogWrite(9, 255);
  analogWrite(10, 255);
}

void loop() {
}
