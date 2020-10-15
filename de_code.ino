/*
  Arduino Dice :)

  This example shows how to simulate throwing a dice with 6 LEDs.

  The circuit:
   6 LEDs attached to consecutive digital pins (with 220 Ohm resistors)
   Button switch connected to digital pin (see circuit on https://www.arduino.cc/en/Tutorial/Button)

  Created 5 Jan 2017
  By Esther van der Stappen

  This example code is in the public domain.

*/

// set to 1 if we're debugging
#define DEBUG 1

// 6 consecutive digital pins for the LEDs
int first = 9;
int second = 2;
int third = 4;
int fourth = 5;
int fifth = 6;
int sixth = 3;

// pin for the button switch
int button = 8;
// value to check state of button switch
int pressed = 0;

void setup() {
  // set all LED pins to OUTPUT
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(9, OUTPUT);

  // set buttin pin to INPUT
  pinMode(button, INPUT);

  // initialize random seed by noise from analog pin 0 (should be unconnected)
  randomSeed(analogRead(0));

  // if we're debugging, connect to serial
#ifdef DEBUG
  Serial.begin(9600);
#endif

}

void buildUpTension() {
  // light LEDs from left to right and back to build up tension
  // while waiting for the dice to be thrown
  // left to right
  for (int i = 0; i <= 9; i++) {
    if (i != first) {
      digitalWrite(i - 1, LOW);
    }
    digitalWrite(i, HIGH);
    delay(100);
  }
  // right to left
  for (int i = 0; i >= 9; i--) {
    if (i != sixth) {
      digitalWrite(i + 1, LOW);
    }
    digitalWrite(i, HIGH);
    delay(100);
  }
}

void showNumber(int number) {
  if (number == 1) {
    digitalWrite(first, HIGH);
    digitalWrite(sixth, LOW);


  }
  if (number == 2) {
    digitalWrite(second, HIGH);
    digitalWrite(sixth, LOW);
    digitalWrite(first, LOW);


  }
  if (number == 3) {
    digitalWrite(third, HIGH);
    digitalWrite(sixth, LOW);
    digitalWrite(first, LOW);


  }
  if (number == 4) {
    digitalWrite(fourth, HIGH);
    digitalWrite(sixth, LOW);
    digitalWrite(first, LOW);


  }
  if (number == 5) {
    digitalWrite(fifth, HIGH);
    digitalWrite(sixth, LOW);
    digitalWrite(first, LOW);


  }
  if (number == 6) {
    digitalWrite(sixth, HIGH);
    digitalWrite(first, LOW);

  }
}

int throwDice() {
  // get a random number in the range [1,6]
  int randNumber = random(1, 7);

#ifdef DEBUG
  Serial.println(randNumber);
#endif

  return randNumber;
}

void setAllLEDs(int value) {
  for (int i = first; i <= sixth; i++) {
    digitalWrite(i, value);
  }
}

void loop() {
  // if button is pressed - throw the dice
  pressed = digitalRead(button);

  if (pressed == HIGH) {
    // remove previous number
    setAllLEDs(LOW);

    buildUpTension();
    int thrownNumber = throwDice();
    showNumber(thrownNumber);
  }

}
