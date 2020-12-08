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

void setup(){
  Serial.begin(9600);
}

void loop(){
  char key = keypad.getKey();
  if (key){
    Serial.println(key);
  }
}
