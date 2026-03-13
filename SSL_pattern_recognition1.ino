#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

// Flex sensor pins
const int thumbPin  = 34;
const int indexPin  = 35;
const int middlePin = 36;
const int ringPin   = 33;
const int pinkyPin  = 25;

int thumb,indexFinger,middle,ring,pinky;
int16_t ax,ay,az,gx,gy,gz;


// ---------- IDEAL VALUES FROM DATASET ----------

struct GestureData
{
String name;
int thumb;
int index;
int middle;
int ring;
int pinky;
int ax;
int ay;
int az;
};

GestureData gestures[] =
{

{"අ",1965,1970,1942,1780,2300,16500,0,2300},

{"ආ",1960,4095,1950,3900,4095,16700,1200,-800},

{"ඇ",1960,4095,4095,3950,4095,15030,-6200,-4200},

{"උ",1960,1968,4095,4095,4095,4700,-15800,-3200},

{"ඒ",1960,1968,4095,4000,4095,13750,-7600,-5600},

{"එ",1960,4095,4095,4050,4095,16000,-4250,-3000}

};

const int gestureCount = 6;


// ---------- SIMILARITY FUNCTION ----------

float similarity(int current,int target,int tolerance)
{

float diff = abs(current-target);

float score = 100 - (diff * 100.0 / tolerance);

if(score < 0) score = 0;

return score;

}



void setup()
{

Serial.begin(115200);

Wire.begin(21,22);

mpu.initialize();

Serial.println("Gesture detection with percentage scoring");

}



void loop()
{

Serial.println("\nCollecting sensor data for 2 seconds...");

unsigned long startTime = millis();

long thumbSum=0,indexSum=0,middleSum=0,ringSum=0,pinkySum=0;

long axSum=0,aySum=0,azSum=0;

int samples=0;



// ---------- COLLECT SENSOR DATA ----------

while(millis()-startTime < 2000)
{

thumb = analogRead(thumbPin);
indexFinger = analogRead(indexPin);
middle = analogRead(middlePin);
ring = analogRead(ringPin);
pinky = analogRead(pinkyPin);

mpu.getMotion6(&ax,&ay,&az,&gx,&gy,&gz);

thumbSum+=thumb;
indexSum+=indexFinger;
middleSum+=middle;
ringSum+=ring;
pinkySum+=pinky;

axSum+=ax;
aySum+=ay;
azSum+=az;

samples++;

delay(20);

}



// ---------- AVERAGE VALUES ----------

thumb = thumbSum/samples;
indexFinger = indexSum/samples;
middle = middleSum/samples;
ring = ringSum/samples;
pinky = pinkySum/samples;

ax = axSum/samples;
ay = aySum/samples;
az = azSum/samples;



// ---------- FIND BEST MATCH ----------

float bestScore = 0;

String bestGesture = "Unknown";


for(int i=0;i<gestureCount;i++)
{

float score = 0;

score += similarity(thumb,gestures[i].thumb,300);
score += similarity(indexFinger,gestures[i].index,600);
score += similarity(middle,gestures[i].middle,600);
score += similarity(ring,gestures[i].ring,600);
score += similarity(pinky,gestures[i].pinky,600);

score += similarity(ax,gestures[i].ax,3000);
score += similarity(ay,gestures[i].ay,3000);
score += similarity(az,gestures[i].az,3000);

score = score / 8;



Serial.print(gestures[i].name);
Serial.print(" score: ");
Serial.println(score);



if(score > bestScore)
{

bestScore = score;

bestGesture = gestures[i].name;

}

}



// ---------- PRINT RESULT ----------

Serial.println("--------------------");

Serial.print("Detected Gesture: ");
Serial.println(bestGesture);

Serial.print("Confidence: ");
Serial.print(bestScore);
Serial.println("%");

Serial.println("--------------------");



// wait before next detection

delay(3000);

}