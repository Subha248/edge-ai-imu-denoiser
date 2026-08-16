#include <Wire.h>          // Include I2C library for ESP32

void setup() {
    Serial.begin(115200);          // Start Serial monitor at 115200 baud
    Wire.begin(21, 22);            // Start I2C: SDA=GPIO21, SCL=GPIO22 (ESP32 default)
    Serial.println("\nI2C Scanner"); // Print header
}

void loop() {
    Serial.println("Scanning..."); // Tell user scan is starting
    int count = 0;                 // Counter for devices found
    
    for (uint8_t addr = 1; addr < 127; addr++) { // Loop all possible I2C addresses
        Wire.beginTransmission(addr);            // Start communication with this address
        if (Wire.endTransmission() == 0) {       // If device responds (ACK)...
            Serial.print("Device found at: 0x"); // Print hex prefix
            if (addr < 16) Serial.print("0");    // Add leading zero for single-digit hex
            Serial.println(addr, HEX);           // Print address in hex (e.g., 68)
            count++;                             // Increase device count
        }
    }
    
    if (count == 0) {                            // If no devices found...
        Serial.println("No I2C devices found");
    } else {                                     // If devices found...
        Serial.print("Total: ");
        Serial.print(count);
        Serial.println(" device(s)");
    }
    
    delay(5000); // Wait 5 seconds before scanning again
}
