//definir les pins
int SensorPin = 0;
//definir les variables
float sensorValue = 0; 

void setup() { 
 // communication serial pour debugging
  Serial.begin(9600);
} 

void loop() { 
 for (int i = 0; i <= 50; i++) //repete le code entre brackets 50x
 { 
   sensorValue = sensorValue + analogRead(SensorPin); //lecture analogue de la valeur
    //addition de toutes les valeurs ensemble pour creer une moyenne
   delay(1); //delai
 } 
 sensorValue = sensorValue/50.0; //moyenne des 50 valeurs
 Serial.println(sensorValue); //affichage sur le moniteur de serie
 delay(30); //delai
} 