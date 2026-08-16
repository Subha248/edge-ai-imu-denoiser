# Edge AI IMU Denoiser

Lightweight repository for collecting, denoising, and running a TinyML model on IMU data from an MPU6050/ESP32 platform.

## Project layout

edge-ai-imu-denoiser/
- README.md (this file)
- LICENSE
- docs/
	- system_architecture.md
	- hardware_setup.md
	- imu_data_acquisition.md
	- performance_metrics.md
- firmware/
	- v1_i2c_scanner/
	- v2_imu_ema_logger/
	- v3_csv_logger/
	- v4_edge_impulse_inference/
- datasets/
	- raw/imu_raw.csv
	- processed/imu_denoised.csv
- results/
	- plots/
	- metrics.txt
- edge_impulse/
	- project_config.json
- models/
	- denoiser_model.tflite

## Quick start

- Open the appropriate Arduino sketch under `firmware/` for your workflow stage.
- Use the Serial Monitor at 115200 baud to view logs and CSV output.
- Collected CSVs go into `datasets/raw/` and are processed into `datasets/processed/`.
- Use the `models/denoiser_model.tflite` for inference workflows.

## Contribution

Create issues or PRs for additional firmware stages, dataset formats, or model improvements.

## Contact

Project maintained by the repository owner.
