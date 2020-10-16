//definir les pins
const int sensorpin = 0;
//definir les variables
int val = 0;

void setup()
{
  // communication serial pour debugging
  Serial.begin(9600);
}
 
void loop()
{
  val = analogRead(sensorpin);       // lecture analogue de la valeur du sensor
  Serial.println(val);            // affiche la valeur sur le moniteur de serie
  delay(400);                    // delai
}