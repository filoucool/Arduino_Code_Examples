int buzzer = 8; 

void setup() {
	pinMode(buzzer, OUTPUT);
}

void loop() {
	for (int i = 0; i < 150; i++) { //fréquence
		digitalWrite(buzzer, HIGH);
		delay(1); // delay 1ms
		digitalWrite(buzzer, LOW); 
		delay(1);
	}
	delay(50);
	for (int j = 0; j < 50; j++) { //fréquence
		digitalWrite(buzzer, HIGH);
		delay(2);
		digitalWrite(buzzer, LOW);
		delay(2);
	}
	delay(100);
}