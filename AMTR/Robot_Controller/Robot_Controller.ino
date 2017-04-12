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
DATA ==> MODE, LOCATION, M1, M2, Conveyor, Sensor Calibration
*/
#include <NewPing.h>
#include <SPI.h>
#include <EEPROM.h>
#include <nRF24L01.h>
#include <RF24.h>
#define CE_PIN   9
#define CSN_PIN 10
#define BUZZ A5
#define MLP 5
#define MLN 6
#define MRP 7
#define MRN 8
#define MCP 0
#define MCN 1
#define Mode 100
#define Location 101
#define CurrentLocation 102
#define LastLocation 103
#define Counter4 104
#define AutoLoc 105
#define REF 106
unsigned long tr = 2000;
unsigned long tb = 1000;
unsigned long tf = 800;
unsigned long tw = 10000;
NewPing sonar1(3, 2, 1000); //(trig,echo,max_distance)
NewPing sonar2(3, 4, 1000);

const uint64_t pipe = 0xE8E8F0F0E1LL; // Define the receiver pipe
RF24 radio(CE_PIN, CSN_PIN);
// ============================= DATA ========================= //
byte data[6];

struct IR {
  unsigned int S[5];
};
IR W;
IR B;
IR R;
void setup()
{

  delay(1000);
  radio.begin();
  radio.openReadingPipe(1, pipe);
  radio.startListening();;
  pinMode(A5, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  EEPROM.get(REF, R);
}

void loop()
{
  Collision();
  ReadData();
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
      delay(500);
      digitalWrite(BUZZ, LOW);
      delay(500);
    }
    EEPROM.update(Mode, 1);
  }
}
/* ====================ROBOT MOTION STARTS HERE===================== */
void Goto(byte location2)
{
  
  /*
  if (location2 == 4)
  {
    if (EEPROM.read(LastLocation) == location2)
      Reached4();
    else
      MovFwd();
  }
  */
  if (EEPROM.read(CurrentLocation) == location2)
    Stop();
  else
    MovFwd();
}
// ======================REACHED4 STARTS HERE========================= //
void Reached4()
{
  while (1)
  {
    switch (EEPROM.read(Counter4))
    {
      case 0:
        Rotate(1);
        EEPROM.update(Counter4, EEPROM.read(Counter4) + 1);
        break;
      case 1:
        MovFwd4();
        break;
      case 2:
        MovFwd4();
        break;
      case 3:
        Conveyor4();
        EEPROM.update(Counter4, EEPROM.read(Counter4) + 1);
        break;
      case 4:
        MovBck4();
        break;
      case 5:
        MovFwd4();
        break;
      case 6:
        Rotate(2);
        EEPROM.update(Counter4, EEPROM.read(Counter4) + 1);
        break;
      case 7:
        MovFwd4();
        break;
      case 8:
        Rotate(1);
        EEPROM.update(Counter4, EEPROM.read(Counter4) + 1);
        break;
      default:
        for (int i = 0; i < 5; i++)
        {
          digitalWrite(BUZZ, HIGH);
          delay(100);
          digitalWrite(BUZZ, LOW);
          delay(100);
        }
    }
  }
}
void MovFwd4()
{
  Collision();
  int SI[] = {analogRead(A0), analogRead(A1), analogRead(A2), analogRead(A3), analogRead(A4)};
  if (SI[0] > R.S[0] && SI[1] > R.S[1] && SI[2] > R.S[2] && SI[3] > R.S[3] && SI[4] > R.S[4])
  {
    digitalWrite(MLP, HIGH);
    digitalWrite(MLN, LOW);
    digitalWrite(MRP, HIGH);
    digitalWrite(MRN, LOW);
    EEPROM.update(Counter4, EEPROM.read(Counter4) + 1);
    delay(tf);

  }
  else
    LFR();
}
void MovBck4()
{
  digitalWrite(MLP, LOW);
  digitalWrite(MLN, HIGH);
  digitalWrite(MRP, LOW);
  digitalWrite(MRN, HIGH);
  delay(tb);
  EEPROM.update(Counter4, EEPROM.read(Counter4) + 1);
}
void Rotate(byte drctn)
{
  if (drctn == 1)
  {
    digitalWrite(MLP, HIGH);
    digitalWrite(MLN, LOW);
    digitalWrite(MRP, LOW);
    digitalWrite(MRN, HIGH);
    delay(tr);
  }
  else
  {
    digitalWrite(MLP, LOW);
    digitalWrite(MLN, HIGH);
    digitalWrite(MRP, HIGH);
    digitalWrite(MRN, LOW);
    delay(tr);
  }
}
void Conveyor4()
{
  while (1)
  {
    ReadData();
    if (data[4] == 1)
    {
      digitalWrite(MCP, HIGH);
      digitalWrite(MCN, LOW);
    }
    else if (data[4] == 2)
    {
      digitalWrite(MCP, LOW);
      digitalWrite(MCN, HIGH);
    }
    else
    {
      digitalWrite(MCP, LOW);
      digitalWrite(MCN, LOW);
      if (EEPROM.read(Location) > 0 && EEPROM.read(Location) < 6)
        break;
    }
  }
}
// ======================Reached 4 Ends Here======================= //
void MovFwd()
{
  Collision();
  int SI[] = {analogRead(A0), analogRead(A1), analogRead(A2), analogRead(A3), analogRead(A4)};
  if (SI[0] > R.S[0] && SI[1] > R.S[1] && SI[2] > R.S[2] && SI[3] > R.S[3] && SI[4] > R.S[4])
  {
    digitalWrite(MLP, HIGH);
    digitalWrite(MLN, LOW);
    digitalWrite(MRP, HIGH);
    digitalWrite(MRN, LOW);
    EEPROM.update(CurrentLocation, EEPROM.read(CurrentLocation) + 1);
    if (EEPROM.read(CurrentLocation) == 6)
      EEPROM.update(CurrentLocation, 1);
    delay(tf);
  }
  else
    LFR();
}
void LFR()
{
  int SI[] = {analogRead(A0), analogRead(A1), analogRead(A2), analogRead(A3), analogRead(A4)};
  //FWD
  if (SI[0] < R.S[0] && SI[1] < R.S[1] && SI[2] > R.S[2] && SI[3] < R.S[3] && SI[4] < R.S[4])
  {
    digitalWrite(MLP, HIGH);
    digitalWrite(MLN, LOW);
    digitalWrite(MRP, HIGH);
    digitalWrite(MRN, LOW);
  }
  //SLIGHT LEFT
  else if ((SI[0] < R.S[0] && SI[1] > R.S[1] && SI[2] > R.S[2] && SI[3] < R.S[3] && SI[4] < R.S[4]) || (SI[0] < R.S[0] && SI[1] > R.S[1] && SI[2] < R.S[2] && SI[3] < R.S[3] && SI[4] < R.S[4]))
  {
    digitalWrite(MLP, LOW);
    digitalWrite(MLN, LOW);
    digitalWrite(MRP, HIGH);
    digitalWrite(MRN, LOW);
  }
  //LEFT
  else if ((SI[0] > R.S[0] && SI[1] < R.S[1] && SI[2] < R.S[2] && SI[3] < R.S[3] && SI[4] < R.S[4]) || (SI[0] > R.S[0] && SI[1] > R.S[1] && SI[2] < R.S[2] && SI[3] < R.S[3] && SI[4] < R.S[4]))
  {
    digitalWrite(MLP, LOW);
    digitalWrite(MLN, HIGH);
    digitalWrite(MRP, HIGH);
    digitalWrite(MRN, LOW);
  }
  //SLIGHT RIGHT
  else if ((SI[0] < R.S[0] && SI[1] < R.S[1] && SI[2] < R.S[2] && SI[3] > R.S[3] && SI[4] < R.S[4]) || (SI[0] < R.S[0] && SI[1] < R.S[1] && SI[2] > R.S[2] && SI[3] > R.S[3] && SI[4] < R.S[4]))
  {
    digitalWrite(MLP, HIGH);
    digitalWrite(MLN, LOW);
    digitalWrite(MRP, LOW);
    digitalWrite(MRN, LOW);
  }
  //RGHT
  else if ((SI[0] < R.S[0] && SI[1] < R.S[1] && SI[2] < R.S[2] && SI[3] > R.S[3] && SI[4] > R.S[4]) || (SI[0] < R.S[0] && SI[1] < R.S[1] && SI[2] < R.S[2] && SI[3] < R.S[3] && SI[4] > R.S[4]))
  {
    digitalWrite(MLP, HIGH);
    digitalWrite(MLN, LOW);
    digitalWrite(MRP, LOW);
    digitalWrite(MRN, HIGH);
  }
  else
  {
    digitalWrite(MLP, HIGH);
    digitalWrite(MLN, LOW);
    digitalWrite(MRP, HIGH);
    digitalWrite(MRN, LOW);
  }
}
/* =====================ROBOT MOTION ENDS HERE====================== */
/* =========================MISC. - START=========================== */
void ReadData()
{
  if ( radio.available() )
  {
    bool done = false;
    while (!done)
    {
      done = radio.read( data, sizeof(data) );
    }
    if ((data[0] > 0 && data[0] < 4) || (data[1] >= 0 && data[1] < 6))
    {
      if (data[1] > 0 && data[1] < 6)
        EEPROM.update(LastLocation, data[1]);
      EEPROM.update(Mode, data[0]);
      EEPROM.update(Location, data[1]);
    }
    else
    {
      for (int i = 0; i < 5; i++)
      {
        digitalWrite(BUZZ, HIGH);
        delay(100);
        digitalWrite(BUZZ, LOW);
        delay(100);
      }
    }

    if (data[5] == 1)
    {
      for (int i = 0; i < 10; i++)
      {
        digitalWrite(BUZZ, HIGH);
        delay(1000);
        digitalWrite(BUZZ, LOW);
        delay(500);
      }
      delay(250);
      for (int i = 14; i <= 18; i++)
      {
        int j = i - 14;
        W.S[j] = analogRead(i);
      }
      delay(250);
      for (int i = 0; i < 10; i++)
      {
        digitalWrite(BUZZ, HIGH);
        delay(500);
        digitalWrite(BUZZ, LOW);
        delay(250);
      }
      delay(250);
      for (int i = 14; i <= 18; i++)
      {
        int j = i - 14;
        B.S[j] = analogRead(i);
      }
      delay(250);
      for (int i = 0; i < 5; i++)
        R.S[i] = (B.S[i] + W.S[i]) / 2;
      EEPROM.put(REF, R);
      digitalWrite(BUZZ, HIGH);
      delay(5000);
      digitalWrite(BUZZ, LOW);
    }
  }
}
byte CheckMode()
{
  byte EMem0 = EEPROM.read(Mode);
  return (EMem0);
}
/* ===========================MISC. - END============================= */
/* =======================MOTOR CONTROL - START======================== */
void Conveyor()
{

  ReadData();
  if (data[4] == 1)
  {
    digitalWrite(MCP, HIGH);
    digitalWrite(MCN, LOW);
  }
  else if (data[4] == 2)
  {
    digitalWrite(MCP, LOW);
    digitalWrite(MCN, HIGH);
  }
  else
  {
    digitalWrite(MCP, LOW);
    digitalWrite(MCN, LOW);
  }
}

void Stop()
{
  digitalWrite(MLP, LOW);
  digitalWrite(MLN, LOW);
  digitalWrite(MRP, LOW);
  digitalWrite(MRN, LOW);
  Conveyor();
}
/* ========================MOTOR CONTROL - END========================= */
/* ==========================SENSORS - START=========================== */

void Collision()
{
  delay(50);
  unsigned long uS1 = sonar1.ping();
  uS1 = uS1 / US_ROUNDTRIP_CM;

  delay(50);
  unsigned long uS2 = sonar2.ping();
  uS2 = uS2 / US_ROUNDTRIP_CM;


  while ((uS2 < 60 && uS2 != 0) || (uS1 < 60 && uS1 != 0))
  {
    delay(50);
    uS1 = sonar1.ping();
    uS1 = uS1 / US_ROUNDTRIP_CM;
    delay(100);
    uS2 = sonar2.ping();
    uS2 = uS2 / US_ROUNDTRIP_CM;
    Stop();
    digitalWrite(BUZZ, HIGH);
    ReadData();
    if (data[2] == 2 && data[3] == 2)
    {
      digitalWrite(MRP, LOW);
      digitalWrite(MRN, HIGH);
      digitalWrite(MLP, LOW);
      digitalWrite(MLN, HIGH);
    }
    else
      Stop();
  }
  digitalWrite(BUZZ, LOW);
}

/* =========================SENSORS - END========================== */
/* =========================MANUAL - START========================= */
void  Manual()
{
  EEPROM.update(CurrentLocation, 0);
  Collision();
  ReadData();
  /* ==============CONTROL FOR LEFT MOTOR============= */

  if (data[2] == 1)
  {
    digitalWrite(MLP, HIGH);
    digitalWrite(MLN, LOW);
  }
  else if (data[2] == 2)
  {
    digitalWrite(MLP, LOW);
    digitalWrite(MLN, HIGH);
  }
  else
  {
    digitalWrite(MLP, LOW);
    digitalWrite(MLN, LOW);
  }
  /* ==============CONTROL FOR RIGHT MOTOR============= */
  if (data[3] == 1)
  {
    digitalWrite(MRP, HIGH);
    digitalWrite(MRN, LOW);
  }
  else if (data[3] == 2)
  {
    digitalWrite(MRP, LOW);
    digitalWrite(MRN, HIGH);
  }
  else
  {
    digitalWrite(MRP, LOW);
    digitalWrite(MRN, LOW);
  }
  Conveyor();
}
/* ==========================MANUAL - END========================== */
/* =====================SEMI AUTONOMOUS - START==================== */
void  SemiAuto()
{
  if (EEPROM.read(CurrentLocation) == 0)
  {
    for (int i = 0; i < 5; i++)
    {
      digitalWrite(MLP, LOW);
      digitalWrite(MLN, LOW);
      digitalWrite(MRP, LOW);
      digitalWrite(MRN, LOW);
      digitalWrite(MCP, LOW);
      digitalWrite(MCN, LOW);
      digitalWrite(BUZZ, HIGH);
      delay(500);
      digitalWrite(BUZZ, LOW);
      delay(500);
    }
    EEPROM.update(CurrentLocation, 1);
    EEPROM.update(LastLocation, 1);
  }
  // Infinite Loop Starts Here //
  while (1)
  {
    Collision();
    ReadData();
    int CM = CheckMode();
    if (CM == 2)
    {
      if (EEPROM.read(Location) == 0)
      {
        if (EEPROM.read(CurrentLocation) == EEPROM.read(LastLocation))
          Stop();
        else
          Cancel();
      }
      else
        Goto(EEPROM.read(Location));
    }
    else if (CM == 1 || CM == 3)
      break;
    else
    {
      for (int i = 1; i < 4; i++)
      {
        digitalWrite(BUZZ, HIGH);
        delay(500);
        digitalWrite(BUZZ, LOW);
        delay(200);
      }
    }
  }
}
//========SEMI AUTONOMOUS FUNCTIONS STARTS HERE========//
void Cancel()
{
  while (1)
  {
    ReadData();
    if (CheckMode() == 2)
    {
      if (EEPROM.read(Location) == 0)
        Goto(1);
      else
        break;
    }
    else
      break;
  }
}
//========SEMI AUTONOMOUS FUNCTIONS ENDS HERE========//
/* =====================SEMI AUTONOMOUS - END====================== */
/* =======================AUTONOMOUS - START======================= */
void  Auto()
{
  if (EEPROM.read(CurrentLocation) == 0)
  {
    for (int i = 0; i < 5; i++)
    {
      digitalWrite(MLP, LOW);
      digitalWrite(MLN, LOW);
      digitalWrite(MRP, LOW);
      digitalWrite(MRN, LOW);
      digitalWrite(MCP, LOW);
      digitalWrite(MCN, LOW);

      digitalWrite(BUZZ, HIGH);
      delay(500);
      digitalWrite(BUZZ, LOW);
      delay(500);
    }
    EEPROM.update(CurrentLocation, 1);
    EEPROM.update(LastLocation, 1);
  }
  EEPROM.update(AutoLoc, 3);
  if (EEPROM.read(CurrentLocation) == 3)
    Goto(5);
  else
    StartSequence();
}
// ====================START SEQUENCE STARTS HERE=================== //
void StartSequence()
{
  while (1)
  {
    ReadData();
    if (EEPROM.read(Mode) != 3)
      break;
    else
    {
      if (EEPROM.read(CurrentLocation) == EEPROM.read(AutoLoc))
      {
        Stop();
        if (EEPROM.read(AutoLoc) == 3)
        {
          EEPROM.update(AutoLoc, 5);
          digitalWrite(BUZZ, HIGH);
          delay(2000);
          digitalWrite(BUZZ, LOW);
          digitalWrite(MCP, HIGH);
          digitalWrite(MCN, LOW);
          delay(tw);
          digitalWrite(MCP, LOW);
          digitalWrite(MCN, LOW);
        }
        else
        {
          EEPROM.update(AutoLoc, 3);
          digitalWrite(BUZZ, HIGH);
          delay(2000);
          digitalWrite(BUZZ, LOW);
          digitalWrite(MCP, LOW);
          digitalWrite(MCN, HIGH);
          delay(tw);
          digitalWrite(MCP, LOW);
          digitalWrite(MCN, LOW);
        }
      }
      else
        Goto(EEPROM.read(AutoLoc));
    }
  }
}
// =====================START SEQUENCE ENDS HERE==================== //
/* ===========================AUTONOMOUS============================ */

