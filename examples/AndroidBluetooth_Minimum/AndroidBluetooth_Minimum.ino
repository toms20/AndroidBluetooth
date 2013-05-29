#include <SoftwareSerial.h>
#include <AndroidBluetooth.h>

#define rxPin 6
#define txPin 7

AndroidBluetooth androidBT(rxPin, txPin);

void setup() {
  Serial.begin(38400);
  androidBT.init();
}

void loop() {
  androidBT.listen();
}
