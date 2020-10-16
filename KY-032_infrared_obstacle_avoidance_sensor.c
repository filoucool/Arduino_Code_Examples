//definir le spins
int pinled = 13; 
int pindetection = 3; 
//definir les variables
int val; 

void setup()
{
  pinMode(pinled, OUTPUT);  //pinled = output
  pinMode(pindetection, INPUT);  //pindetection = input
}

void loop()
{
  val = digitalRead(pindetection); // lecture digitale de la valeur du sensor
  if(val == LOW) // objet detecte = low
  {
    //object detectee
  }
  else
  {
    //aucun object detectee
  }
}