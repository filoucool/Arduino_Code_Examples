#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 4;
char hexKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte row[ROWS] = {9, 8, 7, 6};
byte col[COLS] = {5, 4, 3, 2};
Keypad keypad = Keypad(makeKeymap(hexKeys), row, col, ROWS, COLS);
LiquidCrystal_I2C lcd(0x21, 16, 2);

void setup(){
  lcd.backlight();
  lcd.init();
}

void loop(){
  char key = keypad.getKey();
  if (key){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(key);
  }
}
