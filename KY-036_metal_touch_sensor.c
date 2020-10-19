int Analog = A0;
int Digital = 3;
  
void setup ()
{
  pinMode (Analog_Eingang, INPUT);
  pinMode (Digital_Eingang, INPUT);
  Serial.begin (9600);
}

void loop ()
{
  float Analogval;
  int Digitalval;
  Analogval = analogRead (Analog) * (5.0 / 1023.0); 
  Digitalval = digitalRead (Digital);
  if(Digitalval==1)
  {
  }
  else
  {
  }
  delay (200);
}