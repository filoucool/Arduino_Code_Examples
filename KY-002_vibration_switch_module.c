int shock = 3
int val;

void setup () {
	pinMode (shock, INPUT);
} 

void loop () {
	val = digitalRead (shock);
	if (val == HIGH ) { 
	} 
    else {
	}
}