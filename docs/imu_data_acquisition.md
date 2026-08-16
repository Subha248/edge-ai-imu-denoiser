IMU data acquisition notes

- Use 114 or 400 Hz sampling depending on application.
- Read raw accelerometer and gyroscope registers from the MPU6050.
- Timestamp samples using `millis()` for approximate timing.
- Emit CSV lines with header: `ts,ax,ay,az,gx,gy,gz`.

Best practices:
- Calibrate sensors before recording (offset removal and scale).
- Capture records for motion types of interest (walking, running, idle).
