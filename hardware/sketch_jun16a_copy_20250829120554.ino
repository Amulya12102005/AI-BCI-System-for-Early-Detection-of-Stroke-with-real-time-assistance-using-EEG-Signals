#include <Amulya12-project-1_inferencing.h>  // Your Edge Impulse model header
#include "ei_run_classifier.h"

#define TINY_GSM_MODEM_SIM800

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <TinyGsmClient.h>
#include <HardwareSerial.h>

// ====================== LCD SETUP ======================
LiquidCrystal_I2C lcd(0x27, 16, 2); // Change to 0x3F if needed

// ====================== SIM800L SETUP ======================
#define SerialMon Serial
#define SerialAT  Serial1
#define MODEM_RX 16  // ESP32 RX (connect to SIM800L TX)
#define MODEM_TX 17  // ESP32 TX (connect to SIM800L RX)
#define MODEM_BAUD 9600

TinyGsm modem(SerialAT);

// ====================== EMERGENCY CONFIG ======================
#define EMERGENCY_NUMBER "0000000000"  // Your phone number

bool messageSent = false;

// ====================== Setup ======================
void setup() {
  SerialMon.begin(115200);
  delay(10);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("System Booting...");

  SerialAT.begin(MODEM_BAUD, SERIAL_8N1, MODEM_RX, MODEM_TX);
  delay(3000);
  modem.restart();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Modem Ready");
  delay(2000);
  lcd.clear();
}

// ====================== Stroke Detection ======================
bool isStrokeDetected() {
  const int frameSize = EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE;
  float input_data[frameSize];

  for (int i = 0; i < frameSize; i++) {
    int raw = analogRead(34);  // Connect your EEG analog signal to pin 34
    input_data[i] = (float)raw / 4095.0f;  // Normalize 12-bit ADC
    delay(8);  // 125 Hz sampling rate
  }

  signal_t signal;
  int err = numpy::signal_from_buffer(input_data, frameSize, &signal);
  if (err != 0) {
    SerialMon.println("Signal creation failed");
    return false;
  }

  ei_impulse_result_t result = { 0 };
  EI_IMPULSE_ERROR res = run_classifier(&signal, &result, false);
  if (res != EI_IMPULSE_OK) {
    SerialMon.println("Inference failed");
    return false;
  }

  // Debug print
  for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
    SerialMon.print(result.classification[ix].label);
    SerialMon.print(": ");
    SerialMon.println(result.classification[ix].value, 3);
  }

  // Check if "stroke" probability > 0.7
  for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
    if (strcmp(result.classification[ix].label, "stroke") == 0 &&
        result.classification[ix].value > 0.7) {
      return true;
    }
  }

  return false;
}

// ====================== Main Loop ======================
void loop() {
  bool strokeDetected = isStrokeDetected();

  if (strokeDetected && !messageSent) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Stroke Detected!");
    lcd.setCursor(0, 1);
    lcd.print("Sending SMS...");

    bool sms = modem.sendSMS(EMERGENCY_NUMBER, "ALERT: Possible Brain Stroke Detected!");

    if (sms) {
      SerialMon.println("SMS sent successfully");
      lcd.setCursor(0, 1);
      lcd.print("SMS Sent!");
    } else {
      SerialMon.println("SMS send failed");
      lcd.setCursor(0, 1);
      lcd.print("SMS Failed");
    }

    messageSent = true;
  } 
  else if (!strokeDetected) {
    lcd.setCursor(0, 0);
    lcd.print("Monitoring...    ");
    lcd.setCursor(0, 1);
    lcd.print("No Stroke       ");
    messageSent = false;
  }

  delay(100);  // Wait before next check
}
