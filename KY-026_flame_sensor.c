//definir les pins
int digitalPin = 2;
int analogPin = A0;
//definir les variables
int digitalVal;
int analogVal;

void setup()
{
  pinMode(digitalPin, INPUT); //digitalpin = input
 // communication serial pour debugging
  Serial.begin(9600);
}

void loop()
{
//lecture digitale des valeurs du sensor
  digitalVal = digitalRead(digitalPin); 
  if(digitalVal == HIGH) 
  {
      //flame detectee
  }
  else
  {
      //aucune flame detectee
  }
  // lecture analogue des valeurs du sensor
  analogVal = analogRead(analogPin); 
//affichage sur le moniteur de serie
  Serial.println(analogVal);
  delay(100); //delai
}