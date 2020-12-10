#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#define Password_Length 8 //Password Length (to modify)

char Data[Password_Length];
char Master[Password_Length] = "Password"; //your password (to modify)
byte master_count = 0;
char key;
const byte ROWS = 4;
const byte COLS = 4;
char hexKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};
Keypad keypad = Keypad(makeKeymap(hexKeys), row, col, ROWS, COLS);
LiquidCrystal_I2C lcd(0x21, 16, 2);

void setup(){
  lcd.init();
  lcd.backlight();
}

void loop(){
  lcd.setCursor(0,0);
  lcd.print("Enter Password:");
  key = keypad.getKey();
  if (key){
    Data[master_count] = key;
    lcd.setCursor(master_count,1);
    lcd.print(Data[master_count]);
    master_count++;
    }

  if(master_count == Password_Length-1){
    lcd.clear();
    if(!strcmp(Data, Master)){
      lcd.print("Correct");
    //password is correct, do something
      }
    else{
      lcd.print("Incorrect");
      delay(1000);
      //password incorrect, do something
      }
    lcd.clear();
    clearData();
  }
}

void clearData(){
  while(master_count !=0){
    Data[master_count--] = 0;
  }return;
}
