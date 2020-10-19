int digitalPin = 3; 
int analogPin = A0; 
int digitalVal;
int analogVal; 

void setup()
{
  pinMode(digitalPin, INPUT);
  Serial.begin(9600);
}

void loop()
{
  digitalVal = digitalRead(digitalPin); 
  if(digitalVal == HIGH) // champ magnetique detectee
  {
  }
  else
  {
  }
  analogVal = analogRead(analogPin); 
  Serial.println(analogVal);
  delay(100);
}