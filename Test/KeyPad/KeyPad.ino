#include <Keypad.h>

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

const String password = "1234";
String input_password;

Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

void setup(){
  Serial.begin(9600);
  input_password.reserve(10);
}

void loop(){

  if (NhapMatKhau())
    Serial.println("correct");
  else 
    Serial.println("incorrect");

}

bool NhapMatKhau() {
  while (1) {
      char key = keypad.getKey();

      if (key){
        Serial.println(key);
    
        if(key == '*') {
          input_password = ""; // reset imput password
        }
        else if(key == '#') {
            if(password == input_password) {
              return true;
            } else {
              return false;
            }
      
            input_password = ""; // reset imput password
          } else {
            input_password += key; // append new character to input password string
          }
      }
  } // while (1)
} 
