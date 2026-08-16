#include <Wire.h>

struct IMU_Data {
    int16_t rawX;
    int16_t rawY;
    int16_t rawZ;
};

const float ACCEL_SCALE = 16384.0;
const float GYRO_SCALE  = 131.0;
const float GRAVITY     = 9.81;

// EMA Low-Pass Filter Alpha
const float ALPHA = 0.2;
float filt_ax = 0.0, filt_ay = 0.0, filt_az = 9.81;
float filt_gx = 0.0, filt_gy = 0.0, filt_gz = 0.0;

volatile bool measurementReady = false;

void IRAM_ATTR timerISR() {
    measurementReady = true;
}

void setup() {
    Serial.begin(115200);
    delay(500);

    Wire.begin(21, 22);

    // Handshake
    Wire.beginTransmission(0x68);
    Wire.write(0x75);
    Wire.endTransmission(false);
    Wire.requestFrom(0x68, 1);
    if (Wire.read() != 0x68) {
        while (1) { delay(100); }
    }

    // Wake Up
    Wire.beginTransmission(0x68);
    Wire.write(0x6B);
    Wire.write(0x00);
    Wire.endTransmission();

    // Hardware DLPF 21 Hz
    Wire.beginTransmission(0x68);
    Wire.write(0x1A);
    Wire.write(0x04);
    Wire.endTransmission();

    // Ranges: Accel +-2g, Gyro +-250 deg/s
    Wire.beginTransmission(0x68);
    Wire.write(0x1C);
    Wire.write(0x00);
    Wire.endTransmission();

    Wire.beginTransmission(0x68);
    Wire.write(0x1B);
    Wire.write(0x00);
    Wire.endTransmission();

    // ESP32 Core 3.x Timer (100 Hz / 10ms)
    hw_timer_t *timer = timerBegin(1000000);
    timerAttachInterrupt(timer, &timerISR);
    timerAlarm(timer, 10000, true, 0);
}

void loop() {
    if (measurementReady) {
        measurementReady = false;

        Wire.beginTransmission(0x68);
        Wire.write(0x3B);
        Wire.endTransmission(false);
        Wire.requestFrom(0x68, 14);

        IMU_Data accel, gyro;
        accel.rawX = (Wire.read() << 8) | Wire.read();
        accel.rawY = (Wire.read() << 8) | Wire.read();
        accel.rawZ = (Wire.read() << 8) | Wire.read();
        int16_t tempRaw = (Wire.read() << 8) | Wire.read();
        gyro.rawX  = (Wire.read() << 8) | Wire.read();
        gyro.rawY  = (Wire.read() << 8) | Wire.read();
        gyro.rawZ  = (Wire.read() << 8) | Wire.read();

        // Convert to physical units (m/s^2 and deg/s)
        float raw_ax = (accel.rawX / ACCEL_SCALE) * GRAVITY;
        float raw_ay = (accel.rawY / ACCEL_SCALE) * GRAVITY;
        float raw_az = (accel.rawZ / ACCEL_SCALE) * GRAVITY;
        float raw_gx = gyro.rawX / GYRO_SCALE;
        float raw_gy = gyro.rawY / GYRO_SCALE;
        float raw_gz = gyro.rawZ / GYRO_SCALE;

        // Software EMA LPF
        filt_ax = (ALPHA * raw_ax) + ((1.0 - ALPHA) * filt_ax);
        filt_ay = (ALPHA * raw_ay) + ((1.0 - ALPHA) * filt_ay);
        filt_az = (ALPHA * raw_az) + ((1.0 - ALPHA) * filt_az);
        filt_gx = (ALPHA * raw_gx) + ((1.0 - ALPHA) * filt_gx);
        filt_gy = (ALPHA * raw_gy) + ((1.0 - ALPHA) * filt_gy);
        filt_gz = (ALPHA * raw_gz) + ((1.0 - ALPHA) * filt_gz);

        // CSV Output
        Serial.print(filt_ax, 4); Serial.print(",");
        Serial.print(filt_ay, 4); Serial.print(",");
        Serial.print(filt_az, 4); Serial.print(",");
        Serial.print(filt_gx, 4); Serial.print(",");
        Serial.print(filt_gy, 4); Serial.print(",");
        Serial.println(filt_gz, 4);
    }
}
