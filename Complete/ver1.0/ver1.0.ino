#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
LiquidCrystal_I2C lcd(0x27,16,2);

// --------------------- SETUP STATUS --------------------- //
#define DHT_pin 14                 // Pin out cua cam bien Nhiet do
#define DHT_type DHT11             // Loai DHT11 hoac DHT22
#define PIR_pin 16                 // Chon ngo tin hieu vao cho PIR
#define Buzzer_pin 4
DHT dht(DHT_pin, DHT_type);        // Khoi dong DHT

int PIR_value = 0;                 // set gia tri cua PIR
float Humidity_value = 0;          // set gia tri do am ban dau = 0
float Temperature_value = 0;       // set gia tri nhiet do ban dau = 0
bool flag_LCD = true;
// --------------------- SETUP ARDUINO --------------------- //
void setup() {
  INIT_ARDUINO();
}

unsigned long times = millis();

void loop() {

  DHT_Function();
  PIR_Buzzer();
}

void INIT_ARDUINO() {
  Serial.begin(9600); // Set braud rate
  dht.begin(9600); // Khoi dong DHT
  pinMode(Buzzer_pin, OUTPUT); // Set chan cua Buzzer
  
  lcd.begin(); // Khoi dong lcd                  
  lcd.backlight(); // Bat nen lcd
  lcd.clear();  // Xoa man hinh lcd
}

void DHT_Function() {
  if (millis() - times > 1000) {
    Humidity_value = dht.readHumidity(); // Doc do am
    Temperature_value = dht.readTemperature(); // Doc nhiet do

    times = millis();
  } 
    lcd.setCursor(0, 0);
    lcd.print("Nhiet do: ");
    lcd.setCursor(10, 0);
    lcd.print(Temperature_value);
    lcd.setCursor(0, 1);
    lcd.print("Do am: ");
    lcd.setCursor(10, 1);
    lcd.print(Humidity_value);
}

void PIR_Buzzer() { // Cam bien chuyen dong + canh bao
  PIR_value = digitalRead(PIR_pin);
  if (PIR_value == HIGH) { // Neu gia gia tri o muc cao (1)
    if (flag_LCD) {
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("!!!UNSAFE!!!");
    }
    Buzzer_PIP();
    flag_LCD = false;
    PIR_Buzzer(); // Goi lai ham kiem tra cho den khi An toan
  }
  flag_LCD = true; // Set lai bien flag_LCD nhu ban dau
}

void Buzzer_PIP() {
  tone(Buzzer_pin, 6000, 300);
      delay(10);
  tone(Buzzer_pin, 8000, 300);
      delay(10);
  tone(Buzzer_pin, 5000, 300);
      delay(10);
}
