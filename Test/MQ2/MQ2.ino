
#define pinMQ2 A1


void setup() {
  Serial.begin(9600);
  
}

void loop() {
  int value = analogRead(pinMQ2);
  Serial.println(value);
  delay(1000);
}
