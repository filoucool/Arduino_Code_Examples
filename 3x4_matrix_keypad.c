#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 3;
char hexKeys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte row[ROWS] = {9, 8, 7, 6};
byte col[COLS] = {5, 4, 3};
Keypad keypad = Keypad(makeKeymap(hexKeys), row, col, ROWS, COLS);

void setup(){
  Serial.begin(9600);
}

void loop(){
  char key = keypad.getKey();
  if (key){
    Serial.println(key);
  }
}
