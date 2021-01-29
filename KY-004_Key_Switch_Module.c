int buttonpin = 3;
int val;

void setup()
{
	pinMode(buttonpin,INPUT);
}

void loop()
{
	val = digitalRead(buttonpin);
	if(val==HIGH)
	{
        //button is pressed
	}
	else
	{
        //button is not pressed
	}
}