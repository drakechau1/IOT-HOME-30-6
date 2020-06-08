#include <Servo.h>

Servo myServo;
#define servoPin 5
#define door_close 10
#define door_open 170

void setup() {
  Serial.begin(9600);
  myServo.attach(servoPin);
}

void loop() {
  myServo.write(door_close);
  Serial.println("Close");
  delay(1000);
  myServo.write(door_open);
  Serial.println("Open");
  delay(1000);
}
