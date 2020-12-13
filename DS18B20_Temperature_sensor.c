#include <OneWire.h>

const byte pinBROCHE_ONEWIRE = 7;

enum DS18B20_RCODES {
  READ_OK,  // Lecture ok
  NO_SENSOR_FOUND,
  INVALID_ADDRESS,
  INVALID_SENSOR
  };
OneWire ds(pinBROCHE_ONEWIRE);

byte data[9], addr[8];

if (reset_search) {
  ds.reset_search();
  }
if (!ds.search(addr)) {
  return NO_SENSOR_FOUND;
  }
if (OneWire::crc8(addr, 7) != addr[7]) {
  return INVALID_ADDRESS;
  }
if (addr[0] != 0x28) {
  return INVALID_SENSOR;
  }

ds.reset();
ds.select(addr);
ds.write(0x44, 1);
delay(800);
ds.reset();
ds.select(addr);
ds.write(0xBE);

for (byte i = 0; i < 9; i++) {
  data[i] = ds.read();
  }
*temperature = ((data[1] << 8) | data[0]) * 0.0625;
return READ_OK;
}

void setup() {
Serial.begin(115200);
}

void loop() {
float temperature;
delay(1000);

byte bErr = getTemperature(&temperature, true);
if (bErr != READ_OK) {
  Serial.print("Erreur de lecture du capteur, erreur n° : ");
  Serial.println(bErr);
else {
  Serial.print(F("Temperature : "));
  Serial.print(temperature, 2);
  Serial.println("°C");
  }
}
