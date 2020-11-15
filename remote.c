#include <SoftwareSerial.h>
#include <XBee.h>

SoftwareSerial XBee(2, 3);

int VRx = A0;
int VRy = A1;

int xPosition = 0;
int yPosition = 0;
int mapX;
int mapY;

void setup() {
  Serial.begin(115200);
  XBee.begin(115200);
  pinMode(VRx, INPUT);
  pinMode(VRy, INPUT); 
}

void loop() {
  xPosition = analogRead(VRx);
  yPosition = analogRead(VRy);

  mapX = map(xPosition, 0, 1023, -255, 255); 
  mapY = map(yPosition, 0, 1023, -255, 255);
  
if(mapX>50){
  Serial.print("A");}
  
else if(mapX<-50){
  Serial.print("B");}
  
else if(mapY>200){
  Serial.print("C");}
  
else if(mapY<-200){
  Serial.print("D");}

else if(mapY>-200 and mapY<200 or mapX<-50 and mapX>50){
  Serial.print("f");}
  
Serial.end();
Serial.begin(115200);
delay(100);
}