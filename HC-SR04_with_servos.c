#include <Servo.h>

Servo servo1;
const int servopin = 2

int pos = 0;    

const int trigPin = 3;
const int echoPin = 4;

long duree;
int distance;

void setup() {
pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);
servo1.attach(servopin);
Serial.begin(9600);
}

void loop() {

digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);

duree = pulseIn(echoPin, HIGH);

distance= duree*0.034/2;

Serial.println(distance);

if(distance <= 35){
for (pos = 0; pos <= 180; pos += 1) {
    servo1.write(pos);
    delay(5);
  }
  for (pos = 180; pos >= 0; pos -= 1) {
    servo1.write(pos);
    delay(5);
  }
}