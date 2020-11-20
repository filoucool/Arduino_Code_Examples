int Sensor = 10;
int val;
   
void setup ()
{
  pinMode (Sensor, INPUT) ;
}
   
void loop ()
{
  val = digitalRead (Sensor) ;
   
  if (val == HIGH)
  {
  }
  else
  {
  }
}