#include <Bridge.h>
#include <Temboo.h>
#include "TembooAccount.h" 
                           
const String TWILIO_ACCOUNT_SID = "xxxxxxxxxx";
const String TWILIO_AUTH_TOKEN = "xxxxxxxxxx";
const String TWILIO_NUMBER = "xxxxxxxxxx";
const String RECIPIENT_NUMBER = "xxxxxxxxxx";

boolean attempted = false; 

void setup() {
  Serial.begin(9600);
  delay(4000);
  while(!Serial);
  Bridge.begin();
}

void loop()
{
  if (!attempted) {
    Serial.println("Running SendAnSMS...");
    TembooChoreo SendSMSChoreo;
    SendSMSChoreo.begin();
    SendSMSChoreo.setAccountName(TEMBOO_ACCOUNT);
    SendSMSChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    SendSMSChoreo.setAppKey(TEMBOO_APP_KEY);
    SendSMSChoreo.setChoreo("/Library/Twilio/SMSMessages/SendSMS");
    SendSMSChoreo.addInput("AccountSID", TWILIO_ACCOUNT_SID);
    SendSMSChoreo.addInput("AuthToken", TWILIO_AUTH_TOKEN);
    SendSMSChoreo.addInput("From", TWILIO_NUMBER);
    SendSMSChoreo.addInput("To", RECIPIENT_NUMBER);
    SendSMSChoreo.addInput("Body", "Your message");
    
    unsigned int returnCode = SendSMSChoreo.run();

    if (returnCode == 0) {
        Serial.println("Success! SMS sent!");
    } else {
      while (SendSMSChoreo.available()) {
        char c = SendSMSChoreo.read();
        Serial.print(c);
      }
    } 
    SendSMSChoreo.close();
    attempted=true;
  }
}