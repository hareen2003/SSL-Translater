#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <SparkFunBMI160.h>

Adafruit_ADS1115 ads;
BMI160 imu;

// ===== ESP32 Direct ADC Pins (6 pots)
const int potPins[6] = {34, 35, 32, 33, 36, 39};

// ===== Variables
int espPot[6];
int adsPot[4];

float ax, ay, az;
float gx, gy, gz;

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);

  // ---- Init ADS1115
  if (!ads.begin()) {
    Serial.println("ADS1115 not connected!");
    while (1);
  }
  ads.setGain(GAIN_ONE);  // 4.096V range

  // ---- Init BMI160
  if (imu.begin(BMI160GenClass::I2C_MODE) != BMI160_OK) {
    Serial.println("BMI160 not connected!");
    while (1);
  }

  imu.setAccelRange(2);   // ±2g
  imu.setGyroRange(250);  // ±250 deg/s

  Serial.println("System Ready...");
}

void loop() {

  // ===== Read 6 ESP32 ADC Pots
  for (int i = 0; i < 6; i++) {
    espPot[i] = analogRead(potPins[i]);
  }

  // ===== Read 4 ADS1115 Pots
  adsPot[0] = ads.readADC_SingleEnded(0);
  adsPot[1] = ads.readADC_SingleEnded(1);
  adsPot[2] = ads.readADC_SingleEnded(2);
  adsPot[3] = ads.readADC_SingleEnded(3);

  // ===== Read BMI160
  imu.readAccel();
  imu.readGyro();

  ax = imu.getAccelX();
  ay = imu.getAccelY();
  az = imu.getAccelZ();

  gx = imu.getGyroX();
  gy = imu.getGyroY();
  gz = imu.getGyroZ();

  // ===== Print Data (CSV format for ML)
  Serial.print("P1:"); Serial.print(espPot[0]); Serial.print(",");
  Serial.print("P2:"); Serial.print(espPot[1]); Serial.print(",");
  Serial.print("P3:"); Serial.print(espPot[2]); Serial.print(",");
  Serial.print("P4:"); Serial.print(espPot[3]); Serial.print(",");
  Serial.print("P5:"); Serial.print(espPot[4]); Serial.print(",");
  Serial.print("P6:"); Serial.print(espPot[5]); Serial.print(",");

  Serial.print("P7:"); Serial.print(adsPot[0]); Serial.print(",");
  Serial.print("P8:"); Serial.print(adsPot[1]); Serial.print(",");
  Serial.print("P9:"); Serial.print(adsPot[2]); Serial.print(",");
  Serial.print("P10:"); Serial.print(adsPot[3]); Serial.print(",");

  Serial.print("AX:"); Serial.print(ax); Serial.print(",");
  Serial.print("AY:"); Serial.print(ay); Serial.print(",");
  Serial.print("AZ:"); Serial.print(az); Serial.print(",");

  Serial.print("GX:"); Serial.print(gx); Serial.print(",");
  Serial.print("GY:"); Serial.print(gy); Serial.print(",");
  Serial.print("GZ:"); Serial.println(gz);

  delay(20);  // ~50Hz
}