//definir les pins
const int trigPin = 9;
const int echoPin = 10;
//definir les variables
long duree;
int distance;

void setup() {
// trigpin = output
pinMode(trigPin, OUTPUT); 
// echopin = input
pinMode(echoPin, INPUT); 
// communication serial pour debugging
Serial.begin(9600); 
}

void loop() {
// nettoi le trigpin
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
//trigpin en High pour 10 microsecondes
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
// lecture de echopin, resultat: temps de voyage de l'echo en microsecondes
duration = pulseIn(echoPin, HIGH);
//calcul de la distance
distance= duration*0.034/2;
//affiche la distance sur le moniteur de serie
Serial.print("Distance: ");
Serial.println(distance);
}