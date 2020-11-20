#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <Encoder.h>
String Xvalue, Yvalue;


Encoder knobGauche(2, 4);
Encoder knobDroite(3, 5);

Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x60);
Adafruit_DCMotor *myMotorGauche = AFMS.getMotor(1);
Adafruit_DCMotor *myMotorDroite = AFMS.getMotor(2);

char inbyte_a[10];
int Token, InByte, BufferX, BufferY, inByte;
String mapX, mapY, mapPot1, mapPot2, mapPot3;
int mapXVal, mapYVal, mapPot1Val, mapPot2Val, mapPot3Val;
bool start, finish;

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  AFMS.begin();
  myMotorGauche->setSpeed(150);
  myMotorGauche->run(FORWARD);
  myMotorGauche->run(RELEASE);
  myMotorDroite->setSpeed(150);
  myMotorDroite->run(FORWARD);
  myMotorDroite->run(RELEASE);
  Token = 0;
}

void loop() {
  long newGauche, newDroite;
  static long limit_knob_Gauche = 0;
  static long limit_knob_Droite = 0;

  if (Serial2.available()) {
    //inbyte_a = Serial2.read();
    inByte = Serial2.read();
    Serial.write(inByte);
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
  /*else if((inByte) == 'E'){
    Token = 5;}
    else if((inByte) == ';'){
    Token = 6;}
    else if ((inByte) == 'Z'){
    Token = 7;
    }



  */
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
        myMotorGauche->setSpeed(248);
        myMotorDroite->run(FORWARD);
        myMotorDroite->setSpeed(255);
        Token = 6;}
      } break;
    case 2: { //mapY
        myMotorGauche->run(BACKWARD);
        myMotorGauche->setSpeed(248);
        myMotorDroite->run(BACKWARD);
        myMotorDroite->setSpeed(255);
        Token = 6;
      } break;
    case 3: { //mapPot1
        myMotorGauche->run(BACKWARD);
        myMotorGauche->setSpeed(182);
        myMotorDroite->run(FORWARD);
        myMotorDroite->setSpeed(190);
        limit_knob_Gauche = 30;
        limit_knob_Droite = 30;
        Token = 6;
      } break;
    case 4: { //mapPot2
        myMotorGauche->run(FORWARD);
        myMotorGauche->setSpeed(182 );
        myMotorDroite->run(BACKWARD);
        myMotorDroite->setSpeed(190);
        limit_knob_Gauche = 30;
        limit_knob_Droite = 30;
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
  /*
    case 5:{ //mapPot3
    mapPot3 = mapPot3 + String(inByte);
    }break;
    case 6:{
    finish = true;
    }break;
    case 7:{
    startover();
    }break;
    }

    newGauche = knobGauche.read();
    newDroite = knobDroite.read();

    while (Serial.available()>0)
    {
        if(Xvalue.toInt() < 0){
          myMotorGauche->setSpeed(Xvalue.toInt());
           myMotorGauche->run(FORWARD);
           limit_knob_Gauche=1000;
           myMotorDroite->setSpeed(Xvalue.toInt());
           myMotorDroite->run(FORWARD);
           limit_knob_Droite=1000;
          }
          else if(Xvalue.toInt() > 0){
            while(Xvalue.toInt() < 10){
            myMotorGauche->setSpeed(Xvalue.toInt());
            myMotorGauche->run(BACKWARD);
            limit_knob_Gauche=1000;
            myMotorDroite->setSpeed(Xvalue.toInt());
            myMotorDroite->run(BACKWARD);
            limit_knob_Droite=1000;
            }
           }


      //Y
        if(1=1){// a changer
           myMotorGauche->setSpeed(Yvalue.toInt());
           myMotorGauche->run(FORWARD);
           limit_knob_Gauche=1000;
           myMotorDroite->setSpeed(Yvalue.toInt());
           myMotorDroite->run(FORWARD);
           limit_knob_Droite=1000;
          }
          else if(Yvalue.toInt() > 0){
            while(1=1){ //a changer
            myMotorGauche->setSpeed(Yvalue.toInt());
            myMotorGauche->run(BACKWARD);
            limit_knob_Gauche=1000;
            myMotorDroite->setSpeed(Yvalue.toInt());
            myMotorDroite->run(BACKWARD);
            limit_knob_Droite=1000;
            }}}*/
}
