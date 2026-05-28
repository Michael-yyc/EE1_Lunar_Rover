/*
  Hall Sensor Magnetic Direction Detection
  Adafruit Metro M0
  A1324 Hall Sensor + Potential Divider

  Circuit:
  Hall OUT -> 10k -> A0 junction
  A0 junction -> 20k -> GND

  Hall sensor powered from 5V
  Metro reads reduced 0–3.3V signal

  Features:
  - 12-bit ADC resolution
  - Averaging to reduce noise
  - Automatic baseline calibration
  - Software threshold detection
*/

const int hallPin = A0;

const int numSamples = 100;     // averaging amount
const int threshold = 15;       // sensitivity threshold (ADC counts)

int baseline = 0;

void setup()
{
  Serial.begin(115200);

  // Use full 12-bit ADC resolution
  analogReadResolution(12);

  delay(2000);

  Serial.println("Calibrating baseline...");
  
  baseline = getAverageReading();

  Serial.print("Baseline ADC value: ");
  Serial.println(baseline);

  float baselineVoltage = (baseline * 3.3) / 4095.0;

  Serial.print("Baseline Voltage: ");
  Serial.println(baselineVoltage, 3);

  Serial.println();
}

void loop()
{
  int reading = getAverageReading();

  int difference = reading - baseline;

  float voltage = (reading * 3.3) / 4095.0;

  Serial.print("ADC: ");
  Serial.print(reading);

  Serial.print("   Voltage: ");
  Serial.print(voltage, 3);

  Serial.print(" V   Difference: ");
  Serial.print(difference);

  // Detect magnetic direction
  if(difference > threshold)
  {
    Serial.print("   --> MAGNETIC FIELD: UP");
  }
  else if(difference < -threshold)
  {
    Serial.print("   --> MAGNETIC FIELD: DOWN");
  }
  else
  {
    Serial.print("   --> NO SIGNIFICANT FIELD");
  }

  Serial.println();

  delay(100);
}


// Function to average many readings
int getAverageReading()
{
  long total = 0;

  for(int i = 0; i < numSamples; i++)
  {
    total += analogRead(hallPin);
    delay(1);
  }

  return total / numSamples;
}