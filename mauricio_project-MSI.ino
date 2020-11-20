#include <Servo.h>

Servo servo1, servo2, servo3;
int potentiometer1 = 2, potentiometer2 = 1, potentiometer3 = 0; //analog
long valpot1, valpot2, valpot3;
long x1 = 1, x2 = 1024, y1 = 1, y2 = 256;

void setup() {
  servo1.attach(13); //digital
  servo2.attach(12);
  servo3.attach(11);
  Serial.begin(9600);
}

void loop() {
  valpot1 = analogRead(potentiometer1); //lit valeur potentiometer
  valpot2 = analogRead(potentiometer2);
  valpot3 = analogRead(potentiometer3);
  valpot1 = (valpot1 - x1) * (y2 - y1) / (x2 - x1) + y1;
  valpot2 = (valpot2 - x1) * (y2 - y1) / (x2 - x1) + y1;
  valpot3 = (valpot3 - x1) * (y2 - y1) / (x2 - x1) + y1;

  servo1.write(valpot1); //fait bouger le servo moteur
  servo2.write(valpot2);
  servo3.write(valpot3);

  Serial.print("base");
  Serial.print("#");
  Serial.println(valpot2);
  Serial.print("middle");
  Serial.print("#");
  Serial.println(valpot1);
  Serial.print("top");
  Serial.print("#");
  Serial.println(valpot3);
}
