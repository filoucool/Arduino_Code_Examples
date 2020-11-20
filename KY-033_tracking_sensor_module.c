int Sensor = 10; 
   
void setup ()
{
  Serial.begin(9600); 
  pinMode (Sensor, INPUT) ;
}
   
void loop ()
{
  bool val = digitalRead (Sensor) ; 
   
  if (val == HIGH)
  {
    Serial.println("LineTracker is on the line");
  }
  else
  {
    Serial.println("Linetracker is not on the line");
  }
  Serial.println("------------------------------------");
  delay(500); 
}