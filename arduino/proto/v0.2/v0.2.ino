const int buad_rate = 9600;
const int minr = 100;

const byte
s0 = A0, t0 = 9,
s1 = A1, t1 = 8,
s2 = A2, t2 = 7,
s3 = A3, t3 = 6;

const byte sz = 4;
const byte ss[sz] = {s0, s1, s2, s3};
const byte ts[sz] = {t0, t1, t2, t3};
byte gs[sz] = {0, 0, 0, 0};

void setup() {
  for (auto t : ts) {
    pinMode(t, OUTPUT);
    digitalWrite(t, HIGH);
  }
  Serial.begin(buad_rate);
}

void reset() {
  for (auto t : ts) digitalWrite(t, LOW);
  for (int i = 0; i < sz; ++i) gs[i] = 1;
}

int sumgs() {
  int sum = 0;
  for (int i = 0; i < sz; ++i) sum += gs[i];
  return sum;
}

void loop() {
  if (!sumgs()) reset();

  for (int i = 0; i < sz; ++i)
    if (gs[i] && analogRead(ss[i]) > minr)
      digitalWrite(ts[i], HIGH);

  if (Serial.available() > 0) {
    byte ser = Serial.read();
    switch (ser) {
      // 1,2,3,4 - activate
      case 49: digitalWrite(t0, LOW); break;
      case 50: digitalWrite(t1, LOW); break;
      case 51: digitalWrite(t2, LOW); break;
      case 52: digitalWrite(t3, LOW); break;

      // 0,9,8,7 - deactivate
      case 48: digitalWrite(t0, HIGH); break;
      case 57: digitalWrite(t1, HIGH); break;
      case 56: digitalWrite(t2, HIGH); break;
      case 55: digitalWrite(t3, HIGH); break;

      // R - reset
      case 82: reset(); break;

      default: Serial.write("unsupported operation");
    }
  }

  delay(100);
}

