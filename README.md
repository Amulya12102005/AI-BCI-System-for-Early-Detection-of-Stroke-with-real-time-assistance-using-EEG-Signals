1.Project Title

*AI-BCI System for Early Detection of Stroke with Real-time Assistance using EEG Signals*

2.Introduction

This project is an AI-powered Brain-Computer Interface (BCI) system that detects early signs of stroke using EEG signals in real-time. It uses Edge Impulse ML model, ESP32, and SIM800L GSM module to send emergency alerts via SMS when a possible stroke is detected.

3. Features

-Real-time EEG monitoring using ESP32

-Stroke detection using Edge Impulse ML model

-GSM-based emergency alert system (SIM800L)

-LCD display for system status

-Low-cost and portable design

4. Hardware Requirements

-ESP32

-SIM800L GSM Module

-LCD (I2C 16x2)

-EEG Sensor Module

-Jumper wires, Breadboard, Power Supply

5. Software Requirements

-Arduino IDE

-Edge Impulse SDK

-TinyGSM Library

-LiquidCrystal_I2C Library

6. Circuit Diagram
![Circuit_digram](https://github.com/user-attachments/assets/0fbfbf7c-8c11-4f25-9847-34d88b10da43)

7. How It Works

-EEG signals are read by the ESP32.

-Data is passed into the Edge Impulse ML model deployed on ESP32.

-If stroke probability > 70%, the system triggers an alert.

-The SIM800L sends an SMS to the emergency contact.

-LCD displays the status in real-time.



