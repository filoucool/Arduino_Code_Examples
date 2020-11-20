int sensorpin = 3;
int val;

void setup()
{
	pinMode(sensorpin, INPUT);
}

void loop()
{
	val=digitalRead(sensorpin); 
	if(val == HIGH)
	{
	}
	else
	{
	}
}