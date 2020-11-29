int analogvalue = 0;
long temp = 0;
int Led = D0;
int button =  D2;
int buttonstate = 0;
String buttonstatus;

void setup(){
   pinMode(A0, INPUT);
   pinMode (Led, OUTPUT);
   pinMode (button1, INPUT_PULLDOWN);
    Particle.function("checkTemp", checkHandler);
    Particle.variable("temp", temp);
    Particle.variable("buttonstate", buttonstatus);
    Particle.variable("Temp_Sensor", temp);}

void loop(){
   checkTemp();
   buttonstate = digitalRead(button);
   if (buttonstate == 1){ buttonstatus = "on";}
    else {  buttonstatus = "off";}

void checkTemp(){
  analogvalue = analogRead(A0);
  temp =round(((analogvalue*3.1)/4095) * 100);
   Particle.publish("temp", String(temp), PRIVATE);}

int checkHandler(String command){ checkTemp(); return 1;}

int led1Toggle(String commandLED1) {
 if (commandLED1=="on") {digitalWrite(Led,HIGH); return 1;}
 else if (commandLED1 =="off") {digitalWrite(Led,LOW); return 0;} else { return -1;}}