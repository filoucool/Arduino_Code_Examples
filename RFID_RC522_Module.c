#include <SPI.h>
#include <MFRC522.h>
#define SS_P 10
#define RST_P 9

MFRC522 rfid(SS_P, RST_P);
byte nuidArray[4];

void setup() {
 Serial.begin(9600);
 SPI.begin();
 rfid.PCD_Init();
 Serial.println(F("Scan RFID NUID"));
}

void loop() {
 scan();
      delay(150);
}

void scan()
{
  if ( ! rfid.PICC_IsNewCardPresent())
  return;
  if (  !rfid.PICC_ReadCardSerial())
  return;
  
  if (rfid.uid.uidByte[0] != nuidArray[0] ||
    rfid.uid.uidByte[1] != nuidArray[1] ||
    rfid.uid.uidByte[2] != nuidArray[2] ||
    rfid.uid.uidByte[3] != nuidArray[3] ) {
    Serial.println(F("card detected"));

    for (byte i = 0; i < 4; i++) {
      nuidArray[i] = rfid.uid.uidByte[i];
    }
    Serial.print(F("RFID tag: "));
    printDec(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();
  }
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}

void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
  }
