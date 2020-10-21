#include <Bridge.h>
#include <Temboo.h>
#include "TembooAccount.h"
const String FACEBOOK_ACCESS_TOKEN = "xxxxxxxxxx";

int numRuns = 1;  
int maxRuns = 10; 

void setup() {
  Serial.begin(9600);
  delay(4000);
  while(!Serial);
  Bridge.begin();
}

void loop() {
  if (numRuns <= maxRuns) {
    Serial.println("Running UpdateFacebookStatus - Run #" + String(numRuns++) + "...");
    String statusMsg = "My Arduino Yun has been running for " + String(millis()) + " milliseconds!";
    TembooChoreo SetStatusChoreo;SetStatusChoreo.begin();
    SetStatusChoreo.setAccountName(TEMBOO_ACCOUNT);
    SetStatusChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    SetStatusChoreo.setAppKey(TEMBOO_APP_KEY);
    SetStatusChoreo.setChoreo("/Library/Facebook/Publishing/SetStatus");
    SetStatusChoreo.addInput("AccessToken", FACEBOOK_ACCESS_TOKEN);    
    SetStatusChoreo.addInput("Message", statusMsg);
    unsigned int returnCode = SetStatusChoreo.run();
    Serial.println("Response code: " + String(returnCode));   
    while(SetStatusChoreo.available()) {
      char c = SetStatusChoreo.read();
      Serial.print(c);
    }
    SetStatusChoreo.close();
  }
  Serial.println("Waiting...");
  Serial.println("");
  delay(30000); 
}