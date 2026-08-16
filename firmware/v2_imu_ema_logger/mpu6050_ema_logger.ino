// Simple MPU6050 reader with exponential moving average (EMA) smoothing
#include <Wire.h>

const int MPU_ADDR = 0x68;
float alpha = 0.1; // EMA coefficient
float ax_prev=0, ay_prev=0, az_prev=0;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  delay(100);
  Serial.println("ts,ax,ay,az,gx,gy,gz");
}

void loop() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 14, true);
  int16_t raw_ax = Wire.read()<<8 | Wire.read();
  int16_t raw_ay = Wire.read()<<8 | Wire.read();
  int16_t raw_az = Wire.read()<<8 | Wire.read();
  int16_t raw_gx = Wire.read()<<8 | Wire.read();
  int16_t raw_gy = Wire.read()<<8 | Wire.read();
  int16_t raw_gz = Wire.read()<<8 | Wire.read();

  float ax = raw_ax / 16384.0;
  float ay = raw_ay / 16384.0;
  float az = raw_az / 16384.0;

  // EMA smoothing
  ax_prev = alpha * ax + (1 - alpha) * ax_prev;
  ay_prev = alpha * ay + (1 - alpha) * ay_prev;
  az_prev = alpha * az + (1 - alpha) * az_prev;

  unsigned long ts = millis();
  Serial.printf("%lu,%.6f,%.6f,%.6f,%d,%d,%d\n", ts, ax_prev, ay_prev, az_prev, raw_gx, raw_gy, raw_gz);
  delay(10);
}
