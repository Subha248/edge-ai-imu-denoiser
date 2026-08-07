#include <Wire.h>

void setup() {
  Serial.begin(115200);
  delay(1000);
  Wire.begin();
  Serial.println("ESP32 I2C Scanner");
}

void loop() {
  Serial.println("Scanning I2C bus...");

  for (byte address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    byte error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("Device found at 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
    }
  }

  delay(5000);
}
