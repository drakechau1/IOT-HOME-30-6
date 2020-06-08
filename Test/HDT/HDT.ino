#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27,16,2);

const int DHTPIN = 14;
const int DHTTYPE = DHT11;

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin(9600);
  lcd.begin();                   
  lcd.backlight();
  lcd.clear();
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  lcd.setCursor(0, 0);
  lcd.print("Nhiet do: ");
  lcd.setCursor(10, 0);
  lcd.print(t);
  lcd.setCursor(0, 1);
  lcd.print("Do am: ");
  lcd.setCursor(10, 1);
  lcd.print(h);
  delay(1000);
}
