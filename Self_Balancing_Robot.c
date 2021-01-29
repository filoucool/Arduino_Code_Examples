#include <Arduino_FreeRTOS.h>
#include <SPI.h>
#include <Wire.h>
#include <RF24.h>
#include <nRF24L01.h>

#if (MEGA == 1)
#define Mot1En  2 
#define Mot2En  3 
#define Mot1P1    41
#define Mot1P2    40
#define Mot2P1    43
#define Mot2P2    42
#else
#define Mot1En  3 
#define Mot2En  5 
#define Mot1P1    2
#define Mot1P2    4
#define Mot2P1    6
#define Mot2P2    7
#endif

#define MEGA 0
#if (MEGA == 0)
#define NANO 1
#else
#define NANO 0
#endif

#define DEBUG_MODE 0

#define TASKTEST  0 //durée des tasks
#define TASK1PIN  A0
#define TASK2PIN  A1
#define PWR_MGMT_1    0x6B
#define MPU6050ADDR   0x68  //slave
#define ACCEL_XOUT_H  0x3B

typedef struct
{
  float proportionalValue;
  float integralValue;
  float derivativelValue;
} pidValue_t;

typedef struct
{
  float angleOffset;
  float angleSet; //forward or backward
  int8_t motorTurn;  //left or right
} rxPkt_t;

typedef struct
{
  float angleGet;
  float reserved1;
  int8_t reserved2;
} txPkt_t;

pidValue_t pidValue;
rxPkt_t rxPkt;
txPkt_t txPkt;

float angleErrorNew, angleErrorOld;
void vTaskReceiver(void *pvParameters);
void vTaskController(void *pvParameters);
void calibrateSensor(int* pAccBiasX, int* pAccBiasY, int* pAccBiasZ, int* pGyroBiasX, int* pGyroBiasY, int* pGyroBiasZ)
{
  unsigned int i;
  int accX, accY, accZ, gyroX, gyroY, gyroZ;
  long accXTemp, accYTemp, accZTemp, gyroXTemp, gyroYTemp, gyroZTemp;
  accXTemp = 0;
  accYTemp = 0;
  accZTemp = 0;
  gyroXTemp = 0;
  gyroYTemp = 0;
  gyroZTemp = 0;

  for (i = 0; i <= 1000; i++)
  {
    readSensor(&accX, &accY, &accZ, &gyroX, &gyroY, &gyroZ);
    accXTemp += accX;
    accYTemp += accY;
    accZTemp += accZ;
    gyroXTemp += gyroX;
    gyroYTemp += gyroY;
    gyroZTemp += gyroZ;
    delay(1);
  }
  *pAccBiasX = accXTemp / 1000;
  *pAccBiasY = accYTemp / 1000;
  *pAccBiasZ = accZTemp / 1000 - 16384;
  *pGyroBiasX = gyroXTemp / 1000;
  *pGyroBiasY = gyroYTemp / 1000;
  *pGyroBiasZ = gyroZTemp / 1000;
}

void initMPU6050(void)
{
  Wire.begin();
  Wire.beginTransmission(MPU6050ADDR);
  Wire.write(PWR_MGMT_1);
  Wire.write(0);
  Wire.endTransmission(true);
}

void readSensor(int* pAccX, int* pAccY, int* pAccZ, int* pGyroX, int* pGyroY, int* pGyroZ)
{
  int temperature;
  Wire.beginTransmission(MPU6050ADDR);
  Wire.write(ACCEL_XOUT_H); 
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050ADDR, 14, true);
  *pAccX = Wire.read() << 8 | Wire.read();
  *pAccY = Wire.read() << 8 | Wire.read();
  *pAccZ = Wire.read() << 8 | Wire.read(); 
  temperature = Wire.read() << 8 | Wire.read();
  *pGyroX = Wire.read() << 8 | Wire.read();
  *pGyroY = Wire.read() << 8 | Wire.read();
  *pGyroZ = Wire.read() << 8 | Wire.read();
}

float getAccRoll(int accY, int accBiasY, int accZ, int accBiasZ)
{
  float accRoll;
  accRoll = (atan2((accY - accBiasY), (accZ - accBiasZ))) * RAD_TO_DEG;
  if (accRoll <= 360 && accRoll >= 180) {
    accRoll = 360 - accRoll;
  }
  return accRoll;
}
float getGyroRoll(int gyroX, int gyroBiasX, uint32_t lastTime)
{
  float gyroRoll;
  gyroRoll = ((gyroX - gyroBiasX ) / 131) * ((float)(micros() - lastTime) / 1000000); //FS_SEL=0 131LSB degree/second, according to datasheet
  return gyroRoll;
}

void initMotor()
{
  pinMode(Mot1En, OUTPUT);
  pinMode(Mot2En, OUTPUT);
  pinMode(Mot1P1, OUTPUT);
  pinMode(Mot1P2, OUTPUT);
  pinMode(Mot2P1, OUTPUT);
  pinMode(Mot2P2, OUTPUT);
  digitalWrite(Mot1P1, HIGH);
  digitalWrite(Mot1P2, HIGH);
  digitalWrite(Mot2P1, HIGH);
  digitalWrite(Mot2P2, HIGH);
}

void setup()
{
  #if (DEBUG_MODE == 1)
  Serial.begin(115200);
  #endif
  pidValue.proportionalValue = 1.5;
  pidValue.integralValue = 0.2;
  pidValue.derivativelValue = 2;
  memset(((void*)&txPkt), 0, sizeof(txPkt_t));
  memset(((void*)&rxPkt), 0, sizeof(rxPkt_t));
#if (TASKTEST == 1)
  pinMode(TASK1PIN, OUTPUT);
  pinMode(TASK2PIN, OUTPUT);
  digitalWrite(TASK1PIN, HIGH);
  digitalWrite(TASK2PIN, HIGH);
#endif
  xTaskCreate(
    vTaskReceiver
    ,  (const char *)"Receiver"
    ,  250
    ,  NULL
    ,  
    ,  NULL);
  xTaskCreate(
    vTaskController
    ,  (const char *) "Controller"
    ,  230 
    ,  NULL
    ,  2 
    ,  NULL);
}
void loop()
{
}
void vTaskReceiver(void *pvParameters)
{
#if (MEGA == 1)
  RF24 radio(30, 31);
#else
  RF24 radio(9, 10);
#endif
  uint8_t addresses[][6] = {"1Node", "2Node"};
  bool result;
  rxPkt_t rxPktTemp;
  txPkt_t txPktTemp;
  memset(((void*)&txPktTemp), 0, sizeof(txPkt_t));
  memset(((void*)&rxPktTemp), 0, sizeof(rxPkt_t));
  result = radio.begin();
#if (DEBUG_MODE == 1)
  if (result == true)
    Serial.println("Response from chip");
  else
    Serial.println("No response from chip");
#endif
  result = radio.setDataRate(RF24_2MBPS);
#if (DEBUG_MODE == 1)
  if (result == true)
    Serial.println("Set Data Rate Success");
  else
    Serial.println("Set Data Rate Failed");
#endif
  radio.setPayloadSize(sizeof(rxPkt_t));
  radio.setPALevel(RF24_PA_MAX);
  radio.setAutoAck(false);
  radio.disableCRC();
  radio.maskIRQ(false, false, false);
  radio.setChannel(120);
  radio.setRetries(10, 10);
  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1, addresses[0]);
  radio.startListening();
  for (;;)
  {
#if (TASKTEST == 1)
    digitalWrite(TASK2PIN, LOW);
#endif
    if (radio.available())
    {
#if (DEBUG_MODE == 1)
      Serial.print("Data received\n");
#endif
      while (radio.available())
      {
        radio.read( &rxPktTemp, sizeof(rxPkt_t) );
      }
      radio.stopListening();
      taskENTER_CRITICAL(); 
      rxPkt.angleOffset = rxPktTemp.angleOffset;
      rxPkt.angleSet = rxPktTemp.angleSet;
      rxPkt.motorTurn = rxPktTemp.motorTurn;
      txPktTemp.angleGet = txPkt.angleGet;
      taskEXIT_CRITICAL();
      result = radio.write( &txPktTemp, sizeof(txPkt_t) );
#if (DEBUG_MODE == 1)
      if (result == true)
        Serial.println("Write Success");
      else
        Serial.println("Write Failed");
#endif
      radio.startListening();
#if (DEBUG_MODE == 1)
      Serial.print("angleSet: ");
      Serial.print(rxPkt.angleSet);
      Serial.print("\n");

      Serial.print("angleGet");
      Serial.print(txPkt.angleGet);
      Serial.print("\n");
#endif
    }
#if (TASKTEST == 1)
    digitalWrite(TASK2PIN, HIGH);
#endif
  }
}
#if 1
void vTaskController(void *pvParameters)
{
  int accBiasX, accBiasY, accBiasZ;
  int gyroBiasX, gyroBiasY, gyroBiasZ;
  int accX, accY, accZ, gyroX, gyroY, gyroZ;
  uint32_t lastTime;
  float pPart, iPart, dPart;
  int motorInput;
  initMPU6050();
  calibrateSensor(&accBiasX, &accBiasY, &accBiasZ, &gyroBiasX, &gyroBiasY, &gyroBiasZ);
  angleErrorNew = 0;
  angleErrorOld = 0;
  for (;;)
  {
#if (TASKTEST == 1)
    digitalWrite(TASK1PIN, LOW);
#endif
    readSensor(&accX, &accY, &accZ, &gyroX, &gyroY, &gyroZ);
    txPkt.angleGet = 0.98 * (txPkt.angleGet + getGyroRoll(gyroX, gyroBiasX, lastTime)) + 0.02 * (getAccRoll(accY, accBiasY, accZ, accBiasZ));
    angleErrorOld = angleErrorNew;
    angleErrorNew = txPkt.angleGet - rxPkt.angleSet - rxPkt.angleOffset;
    lastTime = micros(); 
    pPart = pidValue.proportionalValue * angleErrorNew;
    iPart += pidValue.integralValue * angleErrorNew;
    dPart = pidValue.derivativelValue * (angleErrorNew - angleErrorOld);
    motorInput = (int)(pPart + iPart + dPart);
    if (motorInput > 255)
      motorInput = 255;
    else if (motorInput < -255)
      motorInput = -255;
#if (DEBUG_MODE == 1)
    Serial.print(" pPart: ");
    Serial.print(pPart);
    Serial.print(" iPart: ");
    Serial.print(iPart);
    Serial.print(" dPart: ");
    Serial.print(dPart);
    Serial.print(" to motor: ");
    Serial.println(motorInput);
#endif
    if (motorInput > 0)
    { 
      digitalWrite(Mot1P1, LOW);
      digitalWrite(Mot1P2, HIGH);
      digitalWrite(Mot2P1, LOW);
      digitalWrite(Mot2P2, HIGH);
    }
    else if (motorInput < 0) 
    { 
      digitalWrite(Mot1P1, HIGH);
      digitalWrite(Mot1P2, LOW);
      digitalWrite(Mot2P1, HIGH);
      digitalWrite(Mot2P2, LOW);
    }
    if (rxPkt.motorTurn == 1) 
    {
      digitalWrite(Mot1P1, LOW);
      digitalWrite(Mot1P2, HIGH);
      digitalWrite(Mot2P1, HIGH);
      digitalWrite(Mot2P2, LOW);
      motorInput = 255;
    }
    else if (rxPkt.motorTurn == -1)
    {
      digitalWrite(Mot1P1, HIGH);
      digitalWrite(Mot1P2, LOW);
      digitalWrite(Mot2P1, LOW);
      digitalWrite(Mot2P2, HIGH);
      motorInput = 255;
    }
    motorInput = abs(motorInput);
    analogWrite(Mot1En, motorInput);
    analogWrite(Mot2En, motorInput);
#if (DEBUG_MODE == 1)
    Serial.print("Motor1 and Motor2 speed: ");
    Serial.print(motorInput);
    Serial.print("\n");
#endif
#if (TASKTEST == 1)
    digitalWrite(TASK1PIN, HIGH);
#endif
    vTaskDelay(1);
  }
}
#endif