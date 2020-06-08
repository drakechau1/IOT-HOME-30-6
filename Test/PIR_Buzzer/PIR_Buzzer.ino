
#define BuzzerPin 4
#define pinPIR 16 // chon ngo tin hieu vao cho PIR
int val = 0; 
void setup() {
  Serial.begin(9600);
  pinMode(BuzzerPin, OUTPUT);
}

void loop() {
  val = digitalRead(pinPIR);
  if (val == HIGH) // Neu gia gia tri o muc cao (1)
    {
      Serial.println("Motion detected!");
      Buzzer_PIP();
      return;
    }
  Serial.println("Khong co vat the!");
  delay(1000);
}

void Buzzer_PIP() {
  tone(BuzzerPin, 6000, 250);
  delay(125);
  tone(BuzzerPin, 9000, 250);
  delay(250);
  tone(BuzzerPin, 5000, 250);
  delay(125);
}
