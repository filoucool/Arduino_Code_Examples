//REMOTE ET SERIAL READING
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <Encoder.h>
Encoder knobGauche(2, 4);
Encoder knobDroite(3, 5);
Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x60);
Adafruit_DCMotor *myMotorGauche = AFMS.getMotor(1);
Adafruit_DCMotor *myMotorDroite = AFMS.getMotor(2);
int Token, inByte, mapXVal, mapYVal;
String mapX, mapY;
//GPS
#include <Adafruit_GPS.h>
#define GPSSerial Serial3
Adafruit_GPS GPS (&GPSSerial);
#define GPSECHO false
uint32_t timer = millis();
//ultrasonic sensor
const int SonicTrigPin = 13, SonicEchoPin = 12;
long SonicDuration;
int SonicDistance;
//Sharp Infrared Sensor
int InfraSensorPin = 0, InfraSensorValue = 0, InfraSensorCM = 0;
//infrared line tracking sensor
int IR1PIN = 27, IR1VALUE = 0, IR2PIN = 26, IR2VALUE = 0, IR3PIN = 29, IR3VALUE = 0, IR4PIN = 28, IR4VALUE = 0, IR5PIN = 31, IR5VALUE = 0;

void setup() {
  Serial.begin(115200); Serial2.begin(115200); AFMS.begin();
  myMotorGauche->setSpeed(150); myMotorGauche->run(FORWARD); myMotorGauche->run(RELEASE);
  myMotorDroite->setSpeed(150); myMotorDroite->run(FORWARD); myMotorDroite->run(RELEASE);
  Token = 0;
  //GPS
  GPS.begin(115200);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  GPS.sendCommand(PGCMD_ANTENNA);
  delay(1000);
  GPSSerial.println(PMTK_Q_RELEASE);
  //ultrasonic sensor
  pinMode(SonicTrigPin, OUTPUT); pinMode(SonicEchoPin, INPUT);
  pinMode(IR1PIN, INPUT); pinMode(IR2PIN, INPUT); pinMode(IR3PIN, INPUT); pinMode(IR4PIN, INPUT); pinMode(IR5PIN, INPUT);
}

void loop() {
  //REMOTE ET SERIAL READING
  long newGauche, newDroite;
  static long limit_knob_Gauche = 0; 
  static long limit_knob_Droite = 0;

  if (Serial2.available()) {
    //inbyte_a = Serial2.read();
    inByte = Serial2.read();
    Serial.write(inByte);}

  if ((inByte) == 'A') {Token =  1;}
  else if ((inByte) == 'B') {Token = 2;}
  else if ((inByte) == 'C') {Token = 3;}
  else if ((inByte) == 'D') {Token = 4;}
  else if ((inByte) == 'f') {Token = 6;}
  switch (Token) {
    case 1: { 
        while (Token == 1) {
          myMotorGauche->run(FORWARD);
          myMotorGauche->setSpeed(248);
          myMotorDroite->run(FORWARD);
          myMotorDroite->setSpeed(255);
          Token = 6;}} break;
    case 2: {
        myMotorGauche->run(BACKWARD);
        myMotorGauche->setSpeed(248);
        myMotorDroite->run(BACKWARD);
        myMotorDroite->setSpeed(255);
        Token = 6;} break;
    case 3: {
        myMotorGauche->run(BACKWARD);
        myMotorGauche->setSpeed(182);
        myMotorDroite->run(FORWARD);
        myMotorDroite->setSpeed(190);
        //limit_knob_Gauche = 30;
        //limit_knob_Droite = 30;
        Token = 6;} break;
    case 4: { 
        myMotorGauche->run(FORWARD);
        myMotorGauche->setSpeed(182 );
        myMotorDroite->run(BACKWARD);
        myMotorDroite->setSpeed(190);
        //limit_knob_Gauche = 30;
        //limit_knob_Droite = 30;
        Token = 6;} break;
    case 6 : {
        myMotorGauche->run(FORWARD);
        myMotorGauche->setSpeed(1);
        myMotorDroite->run(FORWARD);
        myMotorDroite->setSpeed(1);
        Token = 6;} break;}
  //ultrasonic sensor
  digitalWrite(SonicTrigPin, LOW); delayMicroseconds(2);
  digitalWrite(SonicTrigPin, HIGH); delayMicroseconds(10);
  digitalWrite(SonicTrigPin, LOW);
  SonicDuration = pulseIn(SonicEchoPin, HIGH);
  SonicDistance = SonicDuration * 0.034 / 2;
  Serial.print("SONIC:" + SonicDistance);
  //Sharp Infrared Sensor
  InfraSensorValue = analogRead(A0);
  InfraSensorCM = InfraSensorValue / 2;
  Serial.println("IRSDIST:" + InfraSensorCM);
  //infrared line tracking sensor
  IR1VALUE = digitalRead(IR1PIN); Serial.println("IR1:" + IR1VALUE); IR2VALUE = digitalRead(IR2PIN); Serial.println("IR2:" + IR2VALUE);
  IR3VALUE = digitalRead(IR3PIN); Serial.println("IR3:" + IR3VALUE); IR4VALUE = digitalRead(IR4PIN); Serial.println("IR4:" + IR4VALUE);
  IR5VALUE = digitalRead(IR5PIN); Serial.println("IR5:" + IR5VALUE);
  //GPS
  char c = GPS.read();
  if (GPSECHO)
    if (c) Serial.print(c);
  if (GPS.newNMEAreceived()) {
    Serial.println(GPS.lastNMEA());
    if (!GPS.parse(GPS.lastNMEA()))
      return;
  }
  Serial.println(GPS.milliseconds);
  Serial.print("QUALITY:"); Serial.println((int)GPS.fixquality);
  if (GPS.fix) {
    Serial.print("LAT:");Serial.print(GPS.latitude, 4); Serial.println(GPS.lat);
    Serial.print("LONG:"); Serial.print(GPS.longitude, 4); Serial.println(GPS.lon);
    Serial.print("SPEED:"); Serial.println(GPS.speed);
    Serial.print("ANGLE:"); Serial.println(GPS.angle);
    Serial.print("ALT:"); Serial.println(GPS.altitude);
    Serial.print("SATT: "); Serial.println((int)GPS.satellites);}}
