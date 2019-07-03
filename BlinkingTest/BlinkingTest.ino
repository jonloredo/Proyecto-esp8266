#define D0 16
#define D1 5
#define D2 4
#define D3 0
#define D4 2
#define D5 14
#define D6 12
#define D7 13
#define D8 15

void setup() {

  Serial.begin(115200);
  delay(100);
 pinMode(D1, OUTPUT); // GPIO05, Digital Pin D1
}
void loop() {
 digitalWrite(5, HIGH);
 delay(900);
 digitalWrite(5, LOW);
 delay(500);
}
