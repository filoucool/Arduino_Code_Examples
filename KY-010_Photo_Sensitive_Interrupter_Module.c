int interrupter = 13; 
int val;

void setup()
{
	pinMode(interrupter, INPUT); 
}

void loop()
{
	val=digitalRead(interrupter); 
	if(val == HIGH)
	{
		//interruption
	}
	else
	{
	//pas interruption
	}
}