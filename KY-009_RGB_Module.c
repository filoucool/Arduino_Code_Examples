int Rouge = 10;
int Vert = 11;
int Bleu = 12;
  
void setup ()
{
  pinMode (Rouge, OUTPUT); 
  pinMode (Vert, OUTPUT);
  pinMode (Bleu, OUTPUT); 
}
  
void loop () //Boucle de programme principale
{
  digitalWrite (Rouge, HIGH), digitalWrite (Vert, LOW), digitalWrite (Bleu, LOW);
  delay (500);
  digitalWrite (Rouge, LOW), digitalWrite (Vert, HIGH), digitalWrite (Bleu, LOW);
  delay (500); 
  digitalWrite (Rouge, LOW), digitalWrite (Vert, LOW), digitalWrite (Bleu, HIGH);
  delay (500); 
}