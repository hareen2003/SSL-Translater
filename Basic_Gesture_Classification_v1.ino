// ============================================
// Basic Gesture Classification Prototype
// ============================================

#define POT1 34
#define POT2 35

#define POT3 32
#define POT4 33

#define POT5 25
#define POT6 26

#define POT7 27
#define POT8 14

#define POT9 12
#define POT10 13

int threshold = 2000;  // Adjust after calibration

void setup() {
  Serial.begin(115200);
  Serial.println("Gesture Classification Prototype Started");
}

void loop() {

  int finger1 = analogRead(POT1);
  int finger2 = analogRead(POT3);
  int finger3 = analogRead(POT5);
  int finger4 = analogRead(POT7);
  int finger5 = analogRead(POT9);

  if (finger1 > threshold &&
      finger2 > threshold &&
      finger3 > threshold &&
      finger4 > threshold &&
      finger5 > threshold) {

    Serial.println("Gesture: FIST");
  }
  else if (finger1 < threshold &&
           finger2 < threshold &&
           finger3 < threshold &&
           finger4 < threshold &&
           finger5 < threshold) {

    Serial.println("Gesture: OPEN HAND");
  }
  else {
    Serial.println("Gesture: UNKNOWN");
  }

  delay(1000);
}
