# Edge AI IMU Denoiser

This project builds a compact edge-AI pipeline for denoising inertial measurement unit (IMU) data on an ESP32. The workflow starts with sensor discovery, moves to raw IMU data capture, and eventually targets an on-device inference model that can filter noise in real time.

## Project goals
- Discover and validate the MPU6050 sensor over I2C.
- Read raw accelerometer and gyroscope data from the ESP32.
- Collect data samples for training and testing.
- Deploy a lightweight AI model for edge inference.

## Repository structure
- firmware/: Arduino sketches for each development stage
- docs/: architecture notes and block diagrams descriptions
- datasets/: collected motion samples in CSV form
- models/: trained or placeholder model artifacts

## Development roadmap
1. Sensor scan and wiring validation
2. Raw IMU data logging
3. CSV dataset collection
4. TinyML model deployment and inference

## Getting started
1. Open the Arduino sketch in the desired firmware folder.
2. Upload it to an ESP32 board.
3. Monitor Serial output for sensor readings or logging status.

## Notes
The current repository contains the initial skeleton and working firmware examples for the first two milestones. The later stages are included as placeholders for the CSV logger and Edge Impulse deployment flow.
