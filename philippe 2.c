#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;

const int servo1pin = 2;
const int servo2pin = 3;
const int servo3pin = 4;

int pos1 = 0;
int PosBras = 0;

const int trigPin = 9;
const int echoPin = 10;
const int sonicvar = 0.034;

long duration;
int distance;

void setup() {
    servo1.attach(servo1pin);
    servo2.attach(servo2pin);
    servo3.attach(servo3pin);
    pinmode(trigPin, OUTPUT);
    pinmode(echoPin, INPUT);
    Serial.begin(9600);
}

void loop() {
    digitalWrite(tripPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    Serial.print("duration:");
    Serial.println(duration);
    distance = duration*sonicvar/2;
    Serial.print("distance:");
    Serial.println(distance);

    if(distance <= 40){
     for(PosBras = 0; PosBras <= 120; PosBras += 1){
        servo2.write(PosBras);
        servo3.write(PosBras);
        delay(5);
    }
     for(PosBras = 120; PosBras >= 0; PosBras -= 1){
        servo2.write(PosBras);
        servo3.write(PosBras);
        delay(5);
    }
     for(pos1 = 0; pos1 <= 120; pos1 += 1){
        servo1.write(pos1);
        delay(5);
    }
     for(pos1 = 120; pos1 >= 0; pos1 -= 1){
        servo1.write(pos1);
        delay(5);
    }
    }
}