int laser = 13;

void setup() {                
	pinMode(laserPin, OUTPUT);
}

void loop() {
	digitalWrite(laser, HIGH);
	delay(2000);
	digitalWrite(laser, LOW);
	delay(500); 
}