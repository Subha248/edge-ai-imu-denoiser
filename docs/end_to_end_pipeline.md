1. **Data Acquisition**

* **What:** Uploaded `idle.csv` (resting) and `motion.csv` (moving) data from the **6-axis MPU6050**, sampled at **100 Hz**.

### 2. **Impulse Design**

* **What:** Divided the data into **1000 ms windows**, added a **Spectral DSP Block**, and used two engines: **Neural Network Classifier** and **GMM Anomaly Detector**.

### 3. **Spectral DSP (Feature Extraction)**

* **What:** Removed **gravity (1g)** and electrical noise, performed **FFT (0–50 Hz)**, and extracted **Kurtosis, Skewness, and RMS**.
* **Result:** `idle` (yellow) and `motion` (green) samples were **completely separated with zero overlap**.

### 4. **Classifier Training (Neural Network)**

* **What:** Trained the model for **50 cycles** using the extracted DSP features.
* **Result:** Achieved **100% accuracy** with **0.00 loss** on validation samples.

### 5. **Anomaly Training (GMM)**

* **What:** Selected the top 4 **Kurtosis/Skewness** features and trained a **Gaussian Mixture Model (GMM)**.
* **Result:** Created a **purple safe zone** around normal movement to detect **drops, shocks, and unknown faults**.

### 6. **ESP32 Edge Performance**

* **Latency:** **1 ms** per inference
* **RAM:** **1.4 KB** (<1% of ESP32 SRAM)
* **Flash:** **15.8 KB**
