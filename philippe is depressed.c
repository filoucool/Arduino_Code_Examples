#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;

int pos1 = 0;
int pos23 = 0;


const int servo1pin = 2;
const int servo2pin = 3;
const int servo3pin = 4;

const int trigPin = 5;
const int echoPin = 6;
const int sonicvar = 0.034;

long duration;
int distance;

void setup(){
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    servo1.attach(servo1pin);
    servo2.attach(servo2pin);
    servo3.attach(servo3pin);
    Serial.begin(9600);
}

void loop(){
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    distance = duration*sonicvar/2;

    Serial.println(distance);

    if(distance <= 50){
        for(pos23 = 0; pos23 <= 90; pos23 += 1){
            servo2.write(pos23);
            servo3.write(pos23);
            delay(5);
        }
        delay(1000);
        for(pos23 = 90; pos23 >= 0; pos23 -= 1){
            servo2.write(pos23);
            servo3.write(pos23);
            delay(5);
        }
        FUCKMESIDEWAYS();
    }

    void FUCKMESIDEWAYS(){
        for(int i = 0; i >= 3; i++){
        for(pos1 = 0; pos1 >= 60; pos1 += 1){
            servo1.write(pos1);
            delay(5);
        }
         for(pos1 = 60; pos1 <= 0; pos1 -= 1){
            servo1.write(pos1);
            delay(5);
        }
        }
       // f u
    }
}