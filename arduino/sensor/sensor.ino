#include <ArduinoJson.h>


//
// commands
//
const byte Register = 0;
const int Callback = 1;

const int BaudRate = 115200;

// device uid
const char* uuid = "7d9ee522";

// selenoid pins
const byte sel1 = 1;
const byte sel2 = 2;

// sensor pins
const byte sen1 = 1;
const byte sen2 = 2;

// reading threshold
const int minr = 100;

int cmd = 0;
int read1 = 0;
int read2 = 0;
unsigned long tick = 0;

StaticJsonBuffer<200> jsonBuffer;

void setup() {
  Serial.begin(BaudRate);
  
  JsonObject& root = jsonBuffer.createObject();
  root["id"] = uuid;
  root["cmd"] = 0;
  root["sct"] = 2;
  root["t"] = tick;
  root.printTo(Serial);
}

void loop() {
  if(Serial.available()) {
    JsonObject& root = jsonBuffer.parseObject(Serial.readString());
    if (root.success()) {
      cmd = root["cmd"];
      switch(cmd) {
        case 1: Serial.println("recv cmd-1");
        case 2: Serial.println("recv cmd-2");
        case 3: Serial.println("recv cmd-3");
        case 4: Serial.println("recv cmd-4");
        default:
          Serial.println("invalid command");
      }
    }
  }

  read1 = analogRead(sen1);     
  read2 = analogRead(sen2);

  if(read1 > minr || read2 > minr) {
    tick = millis();
    
    JsonObject& root = jsonBuffer.createObject();
    root["id"] = uuid;
    root["cmd"] = Callback;
    root["t"] = tick;
    
    JsonArray& data = root.createNestedArray("v");
    data.add(read1);
    data.add(read2);
    
    root.printTo(Serial);
  }

  delay(100);
}

