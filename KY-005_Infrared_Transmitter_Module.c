#include <IRremote.h>
IRsend IRS;

void setup()
{
 Serial.begin(9600);
}

void loop() 
{
   for (int i = 0; i < 50; i++) { 
     IRS.sendSony(votre code infrarouge);
     delay(40);
   }
}