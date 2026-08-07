#include <Wire.h>

const int MPU6050_ADDR = 0x68;
const int ACCEL_CONFIG = 0x1C;
const int GYRO_CONFIG = 0x1B;
const int PWR_MGMT_1 = 0x6B;

void writeRegister(byte reg, byte value) {
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission(true);
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Wire.begin();

  writeRegister(PWR_MGMT_1, 0x00);
  writeRegister(ACCEL_CONFIG, 0x00);
  writeRegister(GYRO_CONFIG, 0x00);

  Serial.println("MPU6050 raw reader ready");
}

void loop() {
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_ADDR, 14, true);

  if (Wire.available() >= 14) {
    int16_t ax = (Wire.read() << 8) | Wire.read();
    int16_t ay = (Wire.read() << 8) | Wire.read();
    int16_t az = (Wire.read() << 8) | Wire.read();
    int16_t temp = (Wire.read() << 8) | Wire.read();
    int16_t gx = (Wire.read() << 8) | Wire.read();
    int16_t gy = (Wire.read() << 8) | Wire.read();
    int16_t gz = (Wire.read() << 8) | Wire.read();

    Serial.print("AX:"); Serial.print(ax);
    Serial.print(" AY:"); Serial.print(ay);
    Serial.print(" AZ:"); Serial.print(az);
    Serial.print(" GX:"); Serial.print(gx);
    Serial.print(" GY:"); Serial.print(gy);
    Serial.print(" GZ:"); Serial.println(gz);
  }

  delay(200);
}
