Hardware setup and wiring for the MPU6050 and ESP32

1. Connect VCC to 3.3V on the ESP32.
2. Connect GND to GND.
3. Connect SDA to GPIO21 (default SDA) and SCL to GPIO22 (default SCL) on many ESP32 boards.
4. Pull-ups: if needed, add 4.7k pull-up resistors on SDA and SCL.
5. Check I2C address using the `v1_i2c_scanner` sketch.

Notes:
- Ensure the board uses 3.3V logic.
- Avoid long cable lengths for I2C when sampling IMU at high rates.
