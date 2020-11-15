//REMOTE ET SERIAL READING
#include <Wire.h>
#include <Adafruit_MotorShield.h>
Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x60);
Adafruit_DCMotor *myMotorGauche = AFMS.getMotor(1);
Adafruit_DCMotor *myMotorDroite = AFMS.getMotor(2);
int inByte, Token;
//GPS
#include <Adafruit_GPS.h>
#define GPSSerial Serial3
Adafruit_GPS GPS(&GPSSerial);
#define GPSECHO false
uint32_t timer = millis();
//ultrasonic sensor
#include <HCSR04.h>
HCSR04 hc(13, 12);
//Sharp Infrared Sensor
const int InfraSensorPin = A0;
//infrared line tracking sensor
const int IR1PIN = 27, IR2PIN = 26, IR3PIN = 29, IR4PIN = 28, IR5PIN = 31;

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  AFMS.begin();
  //movement
  myMotorGauche->setSpeed(150);
  myMotorGauche->run(FORWARD);
  myMotorGauche->run(RELEASE);
  myMotorDroite->setSpeed(150);
  myMotorDroite->run(FORWARD);
  myMotorDroite->run(RELEASE);
  //GPS
  /*GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  GPS.sendCommand(PGCMD_ANTENNA);
  delay(1000);
  GPSSerial.println(PMTK_Q_RELEASE);*/
  //ultrasonic sensor
  pinMode(IR1PIN, INPUT);
  pinMode(IR2PIN, INPUT);
  pinMode(IR3PIN, INPUT);
  pinMode(IR4PIN, INPUT);
  pinMode(IR5PIN, INPUT);
}

void loop() {
  //REMOTE ET SERIAL READING
  while (Serial2.available()) {
    inByte = Serial2.read();
  }
    
  if ((inByte) == 'A') {
    Token =  1;
  }
  else if ((inByte) == 'B') {
    Token = 2;
  }
  else if ((inByte) == 'C') {
    Token = 3;
  }
  else if ((inByte) == 'D') {
    Token = 4;
  }
  else if ((inByte) == 'f') {
    Token = 6;
  }
  switch (Token) {
    case 0 :{
      myMotorGauche->run(FORWARD);
        myMotorGauche->setSpeed(100);
        myMotorDroite->run(FORWARD);
        myMotorDroite->setSpeed(100);
        Token = 6;
      }
    case 1: { //mapX
      while(Token == 1){
        myMotorGauche->run(FORWARD);
        myMotorGauche->setSpeed(250);
        myMotorDroite->run(FORWARD);
        myMotorDroite->setSpeed(215);
        Token = 6;}
      } break;
    case 2: { //mapY
        myMotorGauche->run(BACKWARD);
        myMotorGauche->setSpeed(250);
        myMotorDroite->run(BACKWARD);
        myMotorDroite->setSpeed(215);
        Token = 6;
      } break;
    case 3: { //mapPot1
        myMotorGauche->run(BACKWARD);
        myMotorGauche->setSpeed(180);
        myMotorDroite->run(FORWARD);
        myMotorDroite->setSpeed(190);
        Token = 6;
      } break;
    case 4: { //mapPot2
        myMotorGauche->run(FORWARD);
        myMotorGauche->setSpeed(180 );
        myMotorDroite->run(BACKWARD);
        myMotorDroite->setSpeed(190);
        Token = 6;
      } break;
      case 5 :{
      myMotorGauche->run(FORWARD);
        myMotorGauche->setSpeed(50);
        myMotorDroite->run(FORWARD);
        myMotorDroite->setSpeed(50);
        Token = 6;
      }
      case 6 :{
      myMotorGauche->run(FORWARD);
        myMotorGauche->setSpeed(1);
        myMotorDroite->run(FORWARD);
        myMotorDroite->setSpeed(1);
        Token = 6;
      }
     break;
  }
  
  //ultrasonic sensor
  //Serial.println( hc.dist());
  //Sharp Infrared Sensor
  //Serial.print("IRSDIST:");
 //Serial.println( 9462 / (analogRead(A0) - 16.92));
 //Serial.println(map(analogRead(A0),80,490,140,20));
  //infrared line tracking sensor
  Serial.print("IR1:");
  Serial.println(digitalRead(IR1PIN));
  Serial.print("IR2:");
  Serial.println(digitalRead(IR2PIN));
  Serial.print("IR3:");
  Serial.println(digitalRead(IR3PIN));
  Serial.print("IR4:");
  Serial.println(digitalRead(IR4PIN));
  Serial.print("IR5:");
  Serial.println(digitalRead(IR5PIN));
  //GPS
  char c = GPS.read();
  if (GPSECHO)
    if (c);
  if (GPS.newNMEAreceived()) {
    if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
      return;
  }
Serial.print("QUALITY:"); //ok
Serial.println((int)GPS.fixquality);
if (GPS.fix) {
  Serial.print("LAT:"); //ok
  Serial.print(GPS.latitude, 4);
  Serial.println(GPS.lat);
  Serial.print("LONG:"); //ok
  Serial.print(GPS.longitude, 4);
  Serial.println(GPS.lon);
  Serial.print("ALT:"); //ok
  Serial.println(GPS.altitude);
  Serial.print("SATT:"); //ok
  Serial.println((int)GPS.satellites);
}
delay(15);
}