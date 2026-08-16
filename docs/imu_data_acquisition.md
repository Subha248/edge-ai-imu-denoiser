# Project Documentation: Day 2 — IMU Data Acquisition & Edge AI Pipeline Setup

## 1. Objective

* Interface **MPU6050 6-Axis IMU** with **ESP32** using I2C in Wokwi.
* Apply **Hardware DLPF + Software EMA** for noise filtering.
* Collect **6-axis IMU data at 100 Hz**.
* Generate `imu.csv` and upload it to **Edge Impulse Studio** for TinyML training.

---

## 2. Hardware Wiring & Pin Mapping

| MPU6050 | ESP32       | Purpose   |
| ------- | ----------- | --------- |
| **VCC** | **3V3**     | Power     |
| **GND** | **GND**     | Ground    |
| **SDA** | **GPIO 21** | I2C Data  |
| **SCL** | **GPIO 22** | I2C Clock |

---

## 3. Firmware Architecture & Signal Processing

### A. Hardware Configuration

1. **WHO_AM_I (`0x75`)** → Checks whether MPU6050 is connected.
2. **I2C Address = `0x68`**.
3. **PWR_MGMT_1 (`0x6B = 0x00`)** → Wakes the sensor.
4. **CONFIG (`0x1A = 0x04`)** → Enables **DLPF**, approximately 21 Hz cutoff.
5. **Accelerometer (`0x1C = 0x00`)** → Range = **±2g**, scale = **16384 LSB/g**.
6. **Gyroscope (`0x1B = 0x00`)** → Range = **±250°/s**, scale = **131 LSB/°/s**.
7. **ESP32 Timer** → Generates a measurement trigger every **10 ms = 100 Hz**.
8. **Burst Read** → Reads **14 bytes** starting from register `0x3B`.

```text
14 bytes = Accel X(2) + Accel Y(2) + Accel Z(2)
         + Temperature(2)
         + Gyro X(2) + Gyro Y(2) + Gyro Z(2)
```

### B. EMA Software Filter

EMA smooths the sensor readings:

```text
Output(t) = α × Raw(t) + (1 − α) × Output(t−1)
```

**α = 0.2**

Lower α → more smoothing
Higher α → faster response

---

# 4. ESP32 Arduino Core 3.x Code

```cpp
#include <Wire.h>                  // I2C library

struct IMU_Data {
    int16_t rawX;
    int16_t rawY;
    int16_t rawZ;
};

const float ACCEL_SCALE = 16384.0; // ±2g
const float GYRO_SCALE  = 131.0;   // ±250°/s
const float GRAVITY     = 9.81;

const float ALPHA = 0.2;           // EMA smoothing factor

float filt_ax = 0.0, filt_ay = 0.0, filt_az = 9.81;
float filt_gx = 0.0, filt_gy = 0.0, filt_gz = 0.0;

volatile bool measurementReady = false;

// Timer interrupt → tells loop() to read sensor
void IRAM_ATTR timerISR() {
    measurementReady = true;
}

void setup() {

    Serial.begin(115200);           // Start Serial Monitor
    delay(500);

    Wire.begin(21, 22);             // SDA = 21, SCL = 22

    // Check MPU6050
    Wire.beginTransmission(0x68);
    Wire.write(0x75);               // WHO_AM_I register
    Wire.endTransmission(false);
    Wire.requestFrom(0x68, 1);

    if (Wire.read() != 0x68) {
        while (1) { delay(100); }   // Stop if sensor not detected
    }

    // Wake MPU6050
    Wire.beginTransmission(0x68);
    Wire.write(0x6B);               // PWR_MGMT_1
    Wire.write(0x00);
    Wire.endTransmission();

    // Hardware DLPF
    Wire.beginTransmission(0x68);
    Wire.write(0x1A);               // CONFIG
    Wire.write(0x04);               // ~21 Hz
    Wire.endTransmission();

    // Accelerometer ±2g
    Wire.beginTransmission(0x68);
    Wire.write(0x1C);               // ACCEL_CONFIG
    Wire.write(0x00);
    Wire.endTransmission();

    // Gyroscope ±250°/s
    Wire.beginTransmission(0x68);
    Wire.write(0x1B);               // GYRO_CONFIG
    Wire.write(0x00);
    Wire.endTransmission();

    // Timer: 100 Hz = every 10 ms
    hw_timer_t *timer = timerBegin(1000000);
    timerAttachInterrupt(timer, &timerISR);
    timerAlarm(timer, 10000, true, 0);
}

void loop() {

    if (measurementReady) {

        measurementReady = false;

        // Start reading from accelerometer X register
        Wire.beginTransmission(0x68);
        Wire.write(0x3B);
        Wire.endTransmission(false);

        // Read 14 continuous bytes
        Wire.requestFrom(0x68, 14);

        IMU_Data accel, gyro;

        // Read accelerometer
        accel.rawX = (Wire.read() << 8) | Wire.read();
        accel.rawY = (Wire.read() << 8) | Wire.read();
        accel.rawZ = (Wire.read() << 8) | Wire.read();

        // Read temperature
        int16_t tempRaw = (Wire.read() << 8) | Wire.read();

        // Read gyroscope
        gyro.rawX = (Wire.read() << 8) | Wire.read();
        gyro.rawY = (Wire.read() << 8) | Wire.read();
        gyro.rawZ = (Wire.read() << 8) | Wire.read();

        // Convert accelerometer to m/s²
        float raw_ax = (accel.rawX / ACCEL_SCALE) * GRAVITY;
        float raw_ay = (accel.rawY / ACCEL_SCALE) * GRAVITY;
        float raw_az = (accel.rawZ / ACCEL_SCALE) * GRAVITY;

        // Convert gyroscope to °/s
        float raw_gx = gyro.rawX / GYRO_SCALE;
        float raw_gy = gyro.rawY / GYRO_SCALE;
        float raw_gz = gyro.rawZ / GYRO_SCALE;

        // Apply EMA filter
        filt_ax = ALPHA * raw_ax + (1.0 - ALPHA) * filt_ax;
        filt_ay = ALPHA * raw_ay + (1.0 - ALPHA) * filt_ay;
        filt_az = ALPHA * raw_az + (1.0 - ALPHA) * filt_az;

        filt_gx = ALPHA * raw_gx + (1.0 - ALPHA) * filt_gx;
        filt_gy = ALPHA * raw_gy + (1.0 - ALPHA) * filt_gy;
        filt_gz = ALPHA * raw_gz + (1.0 - ALPHA) * filt_gz;

        // Output CSV data
        Serial.print(filt_ax, 4); Serial.print(",");
        Serial.print(filt_ay, 4); Serial.print(",");
        Serial.print(filt_az, 4); Serial.print(",");
        Serial.print(filt_gx, 4); Serial.print(",");
        Serial.print(filt_gy, 4); Serial.print(",");
        Serial.println(filt_gz, 4);
    }
}
```

---

# 5. Dataset Structuring — `imu.csv`

Remove:

* ESP32 bootloader messages such as `rst:0x1...`
* Incomplete/truncated rows

Final format:

```csv
ax,ay,az,gx,gy,gz
0.0000,0.0000,9.8100,0.0000,0.0000,0.0000
1.6677,0.0000,9.8100,0.0000,0.0000,0.0000
...
```

**6 columns = 6 IMU features**

`ax, ay, az, gx, gy, gz`

---

# 6. Edge Impulse CSV Wizard Setup

| Step                      | Setting                      | Purpose                           |
| ------------------------- | ---------------------------- | --------------------------------- |
| **Step 2: Delimiters**    | Comma `,`, Skip lines = `0`  | Reads CSV columns                 |
| **Step 3: Data Topology** | Time-series, Rows = readings | Treats data as sequential signals |
| **Step 3b: Timestamps**   | No timestamp, **100 Hz**     | Creates 10 ms sampling interval   |
| **Step 4: Values**        | `ax, ay, az, gx, gy, gz`     | Selects 6 sensor inputs           |
| **Labels**                | No                           | Dataset has no labels             |
| **Step 5: Segmentation**  | Unlimited                    | Keeps continuous stream           |
| **Upload**                | Data acquisition             | Uploads `imu.csv`                 |
| **Split**                 | 80% Training / 20% Testing   | Creates training/testing data     |

### Final Day 2 Pipeline

```text
MPU6050
   ↓
I2C
   ↓
ESP32
   ↓
Hardware DLPF
   ↓
100 Hz Sampling
   ↓
Raw 6-Axis Data
   ↓
Unit Conversion
   ↓
EMA Filtering
   ↓
CSV Dataset
   ↓
Edge Impulse
   ↓
TinyML Training
```

**One-line revision:**
**MPU6050 → I2C → ESP32 → DLPF → 100 Hz → 6-axis data → EMA → CSV → Edge Impulse → AI training.**
