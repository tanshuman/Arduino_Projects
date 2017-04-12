/*
1 - GND
2 - VCC 3.3V !!! NOT 5V
3 - CE to Arduino pin 9
4 - CSN to Arduino pin 10
5 - SCK to Arduino pin 13
6 - MOSI to Arduino pin 11
7 - MISO to Arduino pin 12
8 - UNUSED
EEPROM:
0 ==> Mode
1 ==> Location
2 ==> Current Location
3 ==> Last Location
4 ==> Counter4
5 ==> AutoLoc
DATA ==> MODE, LOCATION, M1, M2, Conveyor
*/
#include <EEPROM.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#define CE_PIN   9
#define CSN_PIN 10
#define BUZZ A5
const uint64_t pipe = 0xE8E8F0F0E1LL; // Define the transmit pipe
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio
// ============================= DATA ========================= //
byte data[6];
byte VROM[2];
String keys = "123A456B789C*0#D";
boolean key_lockout = false;
void setup()
{
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(pipe);
}
void loop()
{
  int CM = CheckMode();
  if (CM == 1)
    Manual();
  else if (CM == 2)
    SemiAuto();
  else if (CM == 3)
    Auto();
  else
  {
    for (int i = 0; i < 5; i++)
    {
      digitalWrite(BUZZ, HIGH);
      delay(100);
      digitalWrite(BUZZ, LOW);
      delay(100);
    }
    EEPROM.write(0, 1);
  }
  if (EEPROM.read(1) < 0 || EEPROM.read(1) > 5)
  {
    VROM[1] = 0;
    if (EEPROM.read(1) != VROM[1])
      EEPROM.write(1, 0);
  }
  SendData();
}
void SendData()
{
  data[0] = EEPROM.read(0);
  data[1] = EEPROM.read(1);
  radio.write( data, sizeof(data) );
  Serial.print("DATA ==>> ");
  Serial.print(data[0]);
  Serial.print(" ; ");
  Serial.print(data[1]);
  Serial.print(" ; ");
  Serial.print(data[2]);
  Serial.print(" ; ");
  Serial.print(data[3]);
  Serial.print(" ; ");
  Serial.print(data[4]);
  Serial.print(" ; ");
  Serial.println(data[5]);
}
byte CheckMode()
{
  byte EMem0 = EEPROM.read(0);
  return (EMem0);
}
void ChangeMode()
{
  data[2] = 0;
  data[3] = 0;
  data[4] = 0;
  Serial.println("Change Mode");
  while (1)
  {
    int GK = GetKey();
    if (GK == 1)
    {
      Serial.println("Manual");
      VROM[0] = 1;
      if (EEPROM.read(0) != VROM[0])
        EEPROM.write(0, 1);
      break;
    }
    else if (GK == 2)
    {
      Serial.println("Semi Autonomous");
      VROM[0] = 2;
      if (EEPROM.read(0) != VROM[0])
        EEPROM.write(0, 2);
      break;
    }
    else if (GK == 3)
    {
      Serial.println("Autonomous");
      VROM[0] = 3;
      if (EEPROM.read(0) != VROM[0])
        EEPROM.write(0, 3);
      break;
    }
    else if (GK == 0)
    {
      break;
    }
  }
}
byte GetKey()
{
  if (Serial.available())
  {
    char ch = Serial.read();
    if (ch >= '0' && ch <= '9')
    {
      int value = ch - '0';
      return (value);
    }
    else
      return (11);
  }
  else
  {
    char ch = getKey();
    if (ch >= '0' && ch <= '9')
    {
      byte key =(ch - '0');
      return(key);
    }
  }
}

char getKey()
{
  int ret = -1;

  if (analogRead(A0) <= 50)
  {
    delay(50);
    if (analogRead(A0) <= 50)
      key_lockout = false;
  }
  else if (!key_lockout)
  {
    delay(50);
    ret = 15 - (log((analogRead(A0) - 183.9) / 58.24) / 0.1623) + 0.5;
    key_lockout = true;
  }
  if (ret != -1)
    return (keys[ret]);
  else
    return ('E');
  delay(10);
}
/* =========================MANUAL - START========================= */
void  Manual()
{
  VROM[1] = 0;
  if (EEPROM.read(1) != VROM[1])
    EEPROM.write(1, 0);
  switch (GetKey())
  {
    case 0:
      ChangeMode();
    case 1:
      data[2] = 2;
      data[3] = 1;
      break;
    case 2:
      data[2] = 1;
      data[3] = 1;
      break;
    case 3:
      data[2] = 1;
      data[3] = 2;
      break;
    case 4:
      data[2] = 2;
      data[3] = 2;
      break;
    case 5:
      data[4] = 1;
      break;
    case 6:
      data[4] = 2;
      break;
    case 9:
      data[5] = 1;
      break;
    default:
      data[2] = 0;
      data[3] = 0;
      data[4] = 0;
      data[5] = 0;
  }
  SendData();
}
/* ==========================MANUAL - END========================== */
/* =====================SEMI AUTONOMOUS - START==================== */
void  SemiAuto()
{
  VROM[1] = 0;
  if (EEPROM.read(1) != VROM[1])
    EEPROM.write(1, 0);
  while (1)
  {
    if (CheckMode() == 2)
    {
      int GK = GetKey();
      if (GK == 0)
        ChangeMode();
      else if (GK > 0 && GK < 6)
        SendLoc(GK);
      if (GK == 6 || GK == 7)
        data[4] = GK - 5;
      else
        data[4] = 0;
    }
    else
      break;
    SendData();
  }
}
void SendLoc(int key2)
{
  Serial.println(key2);
  if (EEPROM.read(1) == 0 || EEPROM.read(1) == 1)
  {
    VROM[1] = key2;
    if (EEPROM.read(1) != VROM[1])
      EEPROM.write(1, key2);
  }
  else
  {
    if (key2 == EEPROM.read(1))
    {
      VROM[1] = 0;
      if (EEPROM.read(1) != VROM[1])
        EEPROM.write(1, 0);
    }
    else
    {
      digitalWrite(BUZZ, HIGH);
      delay(1000);
      digitalWrite(BUZZ, LOW);
    }
  }
}
/* =====================SEMI AUTONOMOUS - END==================== */
/* ======================AUTONOMOUS - START====================== */
void Auto()
{
  int GK = GetKey();
  if (GK == 0)
    ChangeMode();
  if (GK == 6 || GK == 7)
    data[4] = GK - 5;
  else
    data[4] = 0;
}

