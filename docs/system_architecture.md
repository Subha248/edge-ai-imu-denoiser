# System architecture

The system is organized as a simple edge sensing pipeline.

## Block flow
1. MPU6050 sensor
   - The IMU is connected to the ESP32 over I2C.
   - It provides accelerometer and gyroscope measurements.

2. ESP32 firmware
   - The microcontroller reads sensor values and forwards them to the serial monitor or to a CSV logger.
   - Later versions will run an embedded inference model to denoise the input stream.

3. Data collection
   - Raw samples are stored in CSV format in the datasets folder.
   - These files are used as input for training or validation.

4. Edge AI model
   - A lightweight TensorFlow Lite model is stored in the models folder.
   - The ESP32 uses the model to classify or denoise motion data on-device.

## High-level description
The project starts with hardware validation, then moves through data capture and finally into on-device AI inference. This makes it suitable for low-power embedded applications where sensor data must be processed locally without relying on a cloud service.
