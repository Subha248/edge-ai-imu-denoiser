# Edge AI IMU Denoiser

ESP32 firmware for MPU6050 IMU sampling, Edge Impulse inference, and a live
denoising dashboard.

## Project layout

```text
edge-ai-imu-denoiser/
├── firmware/
│   ├── lib/
│   │   └── subhapradha24-project-1_inferencing/
│   ├── src/
│   │   └── main.cpp
│   └── platformio.ini
├── datasets/
└── README.md
```

Existing CSV datasets are kept in `datasets/`.

## Firmware setup

1. Extract the Edge Impulse Arduino library into
   `firmware/lib/subhapradha24-project-1_inferencing/`. The library is not
   included in this repository yet; the placeholder directory is intentional.
2. Open `firmware/` in PlatformIO and build for the `esp32dev` environment.
3. Connect an MPU6050 to the ESP32 using SDA pin 21 and SCL pin 22.
4. Flash the firmware, connect to the `IMU_AI_NETWORK` Wi-Fi access point
   (password `12345678`), and open `http://192.168.4.1`.

The firmware uses AsyncTCP and ESPAsyncWebServer, which PlatformIO fetches from
the dependencies listed in `firmware/platformio.ini`.
