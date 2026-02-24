// ==========================================
// Wearable Sign Language Glove
// Initial Prototype – Potentiometer Testing
// ==========================================

// Define potentiometer ADC pins
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

void setup() {
  Serial.begin(115200);
  Serial.println("Sign Language Glove - Sensor Test Start");
}

void loop() {

  int p1 = analogRead(POT1);
  int p2 = analogRead(POT2);
  int p3 = analogRead(POT3);
  int p4 = analogRead(POT4);
  int p5 = analogRead(POT5);
  int p6 = analogRead(POT6);
  int p7 = analogRead(POT7);
  int p8 = analogRead(POT8);
  int p9 = analogRead(POT9);
  int p10 = analogRead(POT10);

  Serial.print("Finger1: "); Serial.print(p1); Serial.print(" , "); Serial.println(p2);
  Serial.print("Finger2: "); Serial.print(p3); Serial.print(" , "); Serial.println(p4);
  Serial.print("Finger3: "); Serial.print(p5); Serial.print(" , "); Serial.println(p6);
  Serial.print("Finger4: "); Serial.print(p7); Serial.print(" , "); Serial.println(p8);
  Serial.print("Finger5: "); Serial.print(p9); Serial.print(" , "); Serial.println(p10);

  Serial.println("------------------------------------");

  delay(1000);
}
