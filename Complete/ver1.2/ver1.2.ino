#include <Servo.h>
Servo myServo;
#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
LiquidCrystal_I2C lcd(0x27,16,2);
#include <Keypad.h>

// --------------------- SETUP PIN --------------------- //

#define DHT_pin 14                 // Pin out cua cam bien Nhiet do          // Loai DHT11 hoac DHT22
#define PIR_pin 16                 // Chon ngo tin hieu vao cho PIR
#define Gas_pin 15
#define Buzzer_pin 4
#define servoPin 5


// --------------------- SETUP KEYPAD --------------------- //

const int ROW_NUM = 4; //four rows
const int COLUMN_NUM = 4; //four columns

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3', 'A'},
  {'4','5','6', 'B'},
  {'7','8','9', 'C'},
  {'*','0','#', 'D'}
};

byte pin_rows[ROW_NUM] = {13, 12, 11, 10}; //connect to the row pinouts of the keypad
byte pin_column[COLUMN_NUM] = {9, 8, 7, 6}; //connect to the column pinouts of the keypad


// --------------------- GLOBAL VARIABLE --------------------- //

int PIR_value = 0;                 // set gia tri cua PIR
float Humidity_value = 0;          // set gia tri do am ban dau = 0
float Temperature_value = 0;       // set gia tri nhiet do ban dau = 0
bool flag_LCD = true;
bool flag_Gas = true;
int Gas_value = 0;
const String password = "1234";    // Password
String input_password;
char key;
const int door_close = 10;
const int door_open = 170;
// --------------------- SETUP ARDUINO --------------------- //

DHT dht(DHT_pin, DHT11);           // Khoi dong DHT
Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );
unsigned long times = millis();
unsigned long times_password = millis();
unsigned long times_door = millis();

// ----------------------------------------------------//

void setup() {
  INIT_ARDUINO();
}


void loop() {

  while(NhapMatKhau() == true)
  {
    while(1) {
      
      DHT_Function();
    
      PIR_Buzzer(); // Canh bao chuyen dong
    
      Show_Unsafe_Gas(); 
      
      key = keypad.getKey();
      if (key != NULL) {
        bool flag_logout = false;
        switch (key) {
          case 'B':
            Door();
            break;
          case 'D':
            if (Log_out());
              flag_logout = true;
              break;
        }
        if (flag_logout)
          break;        // Neu flag_logout = true thoat while(1)
      }
      
    }
  }
}

void INIT_ARDUINO() {
  Serial.begin(9600);                 // Set braud rate
  dht.begin(); // Khoi dong DHT
  myServo.attach(servoPin);
  pinMode(Buzzer_pin, OUTPUT);        // Set chan cua Buzzer
  input_password.reserve(10);         // Gioi han mat khau: 10
  lcd.begin();                        // Khoi dong lcd                  
  lcd.backlight();                    // Bat nen lcd
  lcd.clear();                        // Xoa man hinh lcd
}

//----------- HAM DHT -----------//
void DHT_Function() {
  if (millis() - times > 1000) {
    Humidity_value = dht.readHumidity();        // Doc do am
    Temperature_value = dht.readTemperature();  // Doc nhiet do
    Gas_value = analogRead(Gas_pin);
    times = millis();
  } 
      Hien_LCD();
}

//----------- HAM PIR -----------//
void PIR_Buzzer() { // Cam bien chuyen dong + canh bao
  PIR_value = digitalRead(PIR_pin);
  if (PIR_value == HIGH) { // Neu gia gia tri o muc cao (1)
    if (flag_LCD) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Motion detected!");
    }
    Buzzer_PIP();
    flag_LCD = false;
    PIR_Buzzer(); // Goi lai ham kiem tra cho den khi An toan
    lcd.clear();
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

//----------- HAM MQ2 -----------//
bool Check_gas() {
  if (Gas_value > 160) {
    Serial.println(Gas_value);
    return true;
  }
  Serial.println(Gas_value);
  return false;
}

void Show_Unsafe_Gas() {
  if (Check_gas())
  {
    if (flag_Gas) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Gas: Unsafe");
    }
    Buzzer_PIP();
    flag_Gas = false;
    Show_Unsafe_Gas();
  }
  flag_Gas = false;
}

//----------- HAM KEYPAD - PASSWORD -----------//
bool KiemTraMatKhau() {
  input_password = "";
  while (millis() - times_password < 5000) { // Sau 5s, neu nguoi dung khong nhap thi se thoat
      
      key = keypad.getKey();

      if (key){
        times_password = millis(); // set lai times_password
        if(key == '*') {
          input_password = ""; // reset input password
          XuatLCD_notClear(0, 1, input_password);
        }
        else if(key == '#') {
            if(password == input_password) {
              XuatLCD_notClear(0, 1, "Correct");
              delay(1000);
              lcd.clear();
              return true;
            } else {
              XuatLCD_notClear(0, 1, "Incorrect");
              delay(1000);
              lcd.clear();
              return false;
            }
      
            input_password = ""; // reset imput password
          } else {
            input_password += key; // append new character to input password string
          }
          XuatLCD_notClear(0, 1, input_password);
      } 
      
  } // while (1)
  lcd.clear();
  return false; // Neu qua thoi gian thi tra ve false
} 

bool NhapMatKhau() {
    XuatLCD(0, 0, "Nhap mat khau:");
    times_password = millis();
    return KiemTraMatKhau();
}

bool Log_out() {
  
    if (NhapMatKhau()) {
      XuatLCD(0, 0, "Logout!");
      delay(1000);
      lcd.clear();
      return true;
    }
    return false;
}

void Hien_LCD() {

  XuatLCD_notClear(0, 0, "T:");
  XuatLCD_notClear(2, 0, (String)Temperature_value);

  XuatLCD_notClear(8, 0, "H:");
  XuatLCD_notClear(10, 0, (String)Humidity_value);

  XuatLCD_notClear(0, 1, "Gas:");
  XuatLCD_notClear(4, 1, (String)Gas_value);

}

//----------- HAM SERVO -----------//

bool Door_Open() {
  myServo.write(door_open);
  return true;
}

bool Door_Close() {
  myServo.write(door_close);
  return true;
}

void Door() {

    XuatLCD(0, 0, "1:Open  2:Close");
    times_door = millis();
    while (millis() - times_door < 5000) { // Sau 5s khong nhap ban phim thi thoat
      key = keypad.getKey();
      if (key != NULL) {
        times_door = millis();
        switch(key) {
          case '1' :
             Door_Open();
             XuatLCD_notClear(0, 1, "Opened");
             delay(1000);
             break;
          case '2' :
             Door_Close();
             XuatLCD_notClear(0, 1, "Closed");
             delay(1000);
             break;
          default :
             XuatLCD_notClear(0, 1, "Illegal");
             delay(1000);
             lcd.clear();
             Door();
             break;
        } // Switch
      } // if (key != NULL)
    } // while

}

//----------- HAM LCD -----------//
void XuatLCD(int colunm, int row, String msg) {
  lcd.clear();
  lcd.setCursor(colunm, row);
  lcd.print(msg);
  
}

void XuatLCD_notClear(int colunm, int row, String msg) {
  lcd.setCursor(colunm, row);
  lcd.print(msg);
  
}
