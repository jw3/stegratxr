byte t1in = A0,
     t2in = A1,
     t1out = 2,
     t2out = 3;

byte in = 0, out = 0;

int sensorReading = 0;
const int threshold = 100;

void one();
void two();

typedef void (*handler)(void);
handler curr = one;
handler next = two;

void setup() {
  pinMode(t1out, OUTPUT);
  digitalWrite(t1out, HIGH);
  
  pinMode(t2out, OUTPUT);
  digitalWrite(t2out, HIGH);
  
  Serial.begin(9600);
}

void loop() {
  //byte v = 0;
  //for(int i = 0; i < 2; ++i) {
    //if(analogRead(i) < threshold) continue;

int v = analogRead(3);
  if(v > threshold){
    Serial.print("OK-");
    Serial.println(v);
  }
  else if (v > 10) {
    Serial.print("no-");
    Serial.println(v);
  }
  //}
  
  if (Serial.available() > 0) {
    // read the incoming byte:
    byte ser = Serial.read();

    String s = String(ser);
    
    // say what you got:
    Serial.print("I received: ");
    Serial.println(ser, DEC);
    Serial.println(s);

    switch(ser) {
      case 49: digitalWrite(t1out, LOW); break; 
      case 50: digitalWrite(t2out, LOW); break;
      case 51: digitalWrite(t1out, HIGH); break;
      case 52: digitalWrite(t2out, HIGH); break;
      case 53: toggle();
      default: Serial.write("unsupported operation");
    }
  }

  delay(100);
}

//  if(in1 >= threshold) {
//    digitalWrite(t1out, LOW);
//      Serial.write("toggle1");
//  }
//  
//  if(in2 >= threshold) {
//    digitalWrite(t2out, LOW);
//      Serial.write("toggle2");
//  }
//}

void toggle() {
  handler t = next;
  next = curr;
  curr = t;
  Serial.write("toggle");
}

void one() {
  if(check(t1in)) {
    Serial.write("one");
    digitalWrite(t2out, LOW);
    digitalWrite(t1out, HIGH);
  }
}

void two() {
  if(check(t2in)) {
    Serial.write("two");
    digitalWrite(t1out, LOW);
    digitalWrite(t2out, HIGH);
  }
}

byte check(byte in) {
  delay(100);
  sensorReading = analogRead(in);
  if (sensorReading >= threshold) {
    toggle();
    return 1;
  }
  return 0;
}

