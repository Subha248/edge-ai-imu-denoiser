# ESP32 Edge AI IMU Denoiser

A real-time signal cleaning system running directly on an ESP32 microcontroller. It uses an on-device TinyML neural network to detect whether an MPU-6050 sensor is still or moving, dynamically adjusting its noise filtering to eliminate jitter without adding lag.

Includes a self-hosted live web dashboard that plots raw vs. cleaned data over Wi-Fi at 25 FPS without needing an internet connection.

---

## The Problem & The Idea

Low-cost IMUs like the MPU-6050 are prone to electrical noise and micro-vibrations:
- **When sitting still:** Readings jump around due to noise, causing drift when calculating tilt or velocity.
- **When in motion:** A heavy filter smooths out the noise, but adds noticeable delay (lag) during quick turns or movements.

Instead of sticking to one filter setting, this project uses a tiny machine learning model trained on real sensor recordings to constantly check the device's physical state:
- **Idle State:** The system applies aggressive filtering (alpha = 0.02) to lock flat lines and eliminate baseline noise.
- **Motion State:** The system relaxes filtering (alpha = 0.08) to keep tracking responsive and lag-free.

---

## How It Works

1. **Sensor Sampling:** Reads acceleration (Ax, Ay, Az) and angular velocity (Gx, Gy, Gz) over I2C at 50 Hz.
2. **Feature Extraction:** A sliding 100 ms window computes statistical features and FFT spectral bins (78 inputs total).
3. **Neural Network:** An INT8-quantized model trained with Edge Impulse processes the features every 100 ms (~1 ms inference time).
4. **Adaptive Filter:** The motion probability smoothly shifts the filter weight between idle and motion mode.
5. **Web Dashboard:** The ESP32 hosts its own Wi-Fi network and streams dual real-time traces (Raw Red vs. Denoised Green) to any connected phone or browser.

---

## Tech Stack & Hardware

- **MCU:** ESP32 Dev Module (240 MHz dual-core)
- **Sensor:** MPU-6050 6-Axis IMU (I2C @ 400 kHz)
- **Framework:** Arduino / PlatformIO
- **TinyML:** Edge Impulse C++ SDK (quantized INT8 model)
- **Web Interface:** ESPAsyncWebServer & WebSockets running directly on the chip

---

## Hardware Wiring

| MPU-6050 Pin | ESP32 Pin |
| :--- | :--- |
| **VCC** | `3.3V` |
| **GND** | `GND` |
| **SDA** | `GPIO 21` |
| **SCL** | `GPIO 22` |

---

## Repository Structure

```text
edge-ai-imu-denoiser/
├── firmware/
│   ├── lib/
│   │   └── subhapradha24-project-1_inferencing/   # Unzipped Edge Impulse model library
│   ├── src/
│   │   └── main.cpp                               # Main application & web UI code
│   └── platformio.ini                             # Build flags & dependencies
├── datasets/
│   ├── idle_real.json                             # Recorded stationary calibration data
│   └── motion_real.json                           # Recorded dynamic movement data
└── README.md
```

## How to Build & Run
## 1. Build the Code
Open this repository in VS Code with the PlatformIO extension installed.

Extract your exported Edge Impulse model folder into:

```
firmware/lib/subhapradha24-project-1_inferencing/
```
Connect your ESP32 via USB and click Upload.

---
## 2. View the Live Dashboard
On your phone or laptop, look for the Wi-Fi network:

Network Name (SSID): IMU_AI_NETWORK

Password: 12345678

Open any browser and go to:

```
[http://192.168.4.1](http://192.168.4.1)
```
The dark-mode dashboard will load immediately, plotting live raw vs. denoised waveforms across all 6 axes alongside the active AI classification state.
---
