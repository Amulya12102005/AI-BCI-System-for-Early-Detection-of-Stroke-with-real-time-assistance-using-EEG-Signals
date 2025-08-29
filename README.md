# Hardware Code for Stroke Detection System

This folder contains the ESP32 code used to run the real-time stroke detection system.

## Components
- ESP32 Development Board
- SIM800L GSM Module
- LCD Display (16x2 or I2C)
- EEG signals were from the dataset
- Power supply

## Features
- Loads the trained TensorFlow Lite model for inference
- Continuously monitors EEG signals
- Detects stroke-like patterns
- Sends emergency SMS alert via SIM800L

## Upload Instructions
1. Open `sketch_jun16a_copy_20250829120554.ino` in Arduino IDE
2. Install required libraries:
   - `TensorFlowLite_ESP32`
   - `LiquidCrystal_I2C` (for LCD)
   - `SoftwareSerial` (for SIM800L)
3. Select **ESP32 Dev Module** board
4. Upload the code
