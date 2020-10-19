//definir les pins
int ThermistorPin = A0;
//definir les variables
int Vo;
float R1 = 10000; //valeur du board R1
float logR2, R2, T;
float c1 = 0.001129148, c2 = 0.000234125, c3 = 0.0000000876741; //coeficients steinhart-hart pour thermistor

void setup() {
 // communication serial pour debugging
  Serial.begin(9600);
}

void loop() {
  Vo = analogRead(ThermistorPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0); //calcul de la ressitance sur thermistor
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2)); //temperature en kelvin
  T = T - 273.15; //convertion vers celcius
  Serial.print(T); //affichage sur le moniteur de serie
  Serial.println(" C"); 
  delay(500); //delai
}