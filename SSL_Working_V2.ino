#include <Wire.h>
#include <MPU6050.h>
#include <WiFi.h>
#include <WebSocketsServer.h>

MPU6050 mpu;

// Flex sensor pins
const int thumbPin  = 34;
const int indexPin  = 35;
const int middlePin = 36;
const int ringPin   = 33;
const int pinkyPin  = 25;

int thumb, indexFinger, middle, ring, pinky;
int16_t ax, ay, az, gx, gy, gz;

// Gesture dataset
struct GestureData {
  String name;
  int thumb,index,middle,ring,pinky;
  int ax,ay,az;
};

GestureData gestures[] = {
  {"අ",1965,1970,1942,1780,2300,16500,0,2300},
  {"ආ",1960,4095,1950,3900,4095,16700,1200,-800},
  {"ඇ",1960,4095,4095,3950,4095,15030,-6200,-4200},
  {"ඒ",1960,1968,4095,4000,4095,13750,-7600,-5600},
  {"උ",1960,1968,4095,4095,4095,4700,-15800,-3200},
  {"එ",1960,4095,4095,4050,4095,16000,-4250,-3000}
};
const int gestureCount = 6;

// Wi-Fi AP settings
const char* apSSID = "GloveAP";
const char* apPassword = "12345678";

// WebSocket server on port 81
WebSocketsServer webSocket = WebSocketsServer(81);

// Similarity function
float similarity(int current,int target,int tolerance){
  float diff = abs(current-target);
  float score = 100 - (diff*100.0/tolerance);
  return score<0?0:score;
}

void setup() {
  Serial.begin(115200);
  Wire.begin(21,22);
  mpu.initialize();

  // Start Wi-Fi Access Point
  WiFi.softAP(apSSID, apPassword);
  Serial.println("ESP32 Access Point started!");
  Serial.print("Connect your PC to Wi-Fi: "); Serial.println(apSSID);
  Serial.print("ESP32 IP: "); Serial.println(WiFi.softAPIP()); // usually 192.168.4.1

  // Start WebSocket
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Serial.println("WebSocket server started on port 81");
}

// WebSocket event handler
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length){
  // Optional: handle messages from dashboard
}

void loop() {
  webSocket.loop();

  // Read sensors
  thumb       = analogRead(thumbPin);
  indexFinger = analogRead(indexPin);
  middle      = analogRead(middlePin);
  ring        = analogRead(ringPin);
  pinky       = analogRead(pinkyPin);

  mpu.getMotion6(&ax,&ay,&az,&gx,&gy,&gz);

  // Detect gesture
  float bestScore = 0;
  String bestGesture = "Unknown";

  for(int i=0;i<gestureCount;i++){
    float score = 0;
    score += similarity(thumb,gestures[i].thumb,300);
    score += similarity(indexFinger,gestures[i].index,800);
    score += similarity(middle,gestures[i].middle,800);
    score += similarity(ring,gestures[i].ring,800);
    score += similarity(pinky,gestures[i].pinky,800);
    score += similarity(ax,gestures[i].ax,4000);
    score += similarity(ay,gestures[i].ay,4000);
    score += similarity(az,gestures[i].az,4000);
    score = score/8;
    if(score>bestScore){
      bestScore = score;
      bestGesture = gestures[i].name;
    }
  }

  // Send JSON via WebSocket
  String json = "{\"gesture\":\""+bestGesture+"\",\"confidence\":"+String(bestScore)+
                ",\"thumb\":"+String(thumb)+
                ",\"index\":"+String(indexFinger)+
                ",\"middle\":"+String(middle)+
                ",\"ring\":"+String(ring)+
                ",\"pinky\":"+String(pinky)+
                ",\"ax\":"+String(ax)+
                ",\"ay\":"+String(ay)+
                ",\"az\":"+String(az)+"}";

  webSocket.broadcastTXT(json);

  delay(200); // 5 updates/sec
}