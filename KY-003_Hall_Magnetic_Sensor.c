int kY003 = 3;
int value;

void setup()
{
	pinMode(kY003, INPUT);
}

void loop()
{
	val = digitalRead(kY003);
	if(value == LOW)
	{
  //do something if magnetic field is strong
	}
	else
	{
  //do something is magnetic field is low
	}
}
