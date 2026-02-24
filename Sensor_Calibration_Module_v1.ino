// ============================================
// Sensor Calibration Module
// ============================================

#define POT1 34
#define POT2 35
#define POT3 32
#define POT4 33
#define POT5 25

int minVal[5] = {4095,4095,4095,4095,4095};
int maxVal[5] = {0,0,0,0,0};

void setup() {
  Serial.begin(115200);
  Serial.println("Calibration Started...");
}

void loop() {

  int values[5];

  values[0] = analogRead(POT1);
  values[1] = analogRead(POT2);
  values[2] = analogRead(POT3);
  values[3] = analogRead(POT4);
  values[4] = analogRead(POT5);

  for(int i = 0; i < 5; i++) {

    if(values[i] < minVal[i])
      minVal[i] = values[i];

    if(values[i] > maxVal[i])
      maxVal[i] = values[i];

    int normalized = map(values[i], minVal[i], maxVal[i], 0, 100);

    Serial.print("Finger ");
    Serial.print(i+1);
    Serial.print(" : ");
    Serial.print(normalized);
    Serial.print("%  ");
  }

  Serial.println();
  delay(1000);
}
