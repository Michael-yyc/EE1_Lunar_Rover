/*
  A1324 Hall Sensor Magnetic Direction Detection

  Circuit:
  Hall OUT -> 10k -> 20k -> GND
  A0 pin at node between 10k and 20k

  Hall sensor powered from 5V
  Metro reads reduced 0–3.3V signal

  Features:
  - 12-bit ADC resolution
  - Averaging to reduce noise
  - Automatic baseline calibration
*/

#include <Arduino.h>

const int hallPin = A0;
const int numSamples = 100; // averaging amount
const int threshold = 375; // sensitivity threshold (ADC counts)


// function to average 100 readings
int average_reading()
{
  long total = 0;

  for (int i = 0; i < numSamples; i++) {
    total += analogRead(hallPin);
    delay(1);
  }

  return total / numSamples;
}


int baseline;

void setup()
{
  Serial.begin(9600);

  //12-bit ADC resolution
  analogReadResolution(12);

  delay(2000);

  Serial.println("Calibrating baseline...");
  
  baseline = average_reading();

  Serial.print("Baseline ADC value: ");
  Serial.println(baseline);

  float baselineVoltage = (baseline * 3.3) / 4095.0; //converts binary ADC count to voltage between 0 - 3.3V

  Serial.print("Baseline Voltage: ");
  Serial.println(baselineVoltage, 3); //printed to 3 d.p

  Serial.println();
}

void loop()
{
  int reading = average_reading();

  int deviation = reading - baseline;

  float voltage = (reading * 3.3) / 4095.0; //converts binary ADC count to voltage between 0 - 3.3V

  Serial.print("ADC: ");
  Serial.print(reading);

  Serial.print("  Voltage: ");
  Serial.print(voltage, 3); //printed to 3 d.p

  Serial.print("  Deviation: ");
  Serial.print(deviation);

  //detect magnetic direction
  if (deviation > threshold) {
    Serial.print("   --> MAGNETIC FIELD: UP");
  }
  else if (deviation < -threshold) {
    Serial.print("   --> MAGNETIC FIELD: DOWN");
  }
  else {
    Serial.print("   --> NO FIELD DETECTED");
  }

  Serial.println();

  delay(100);
}