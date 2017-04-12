byte alarmState;
int s1r, s2r;
String keys = "123A456B789C*0#D";
boolean key_lockout = false;
unsigned long pass = 1234;
int index = 0;
unsigned long entPass = 0;
boolean dl = 10;

#include <NewPing.h>
#include <EEPROM.h>
#include<LiquidCrystal.h>
NewPing sonar(3, 2, 1000); //(trig,echo,max_distance)
NewPing sonar2(3, 4, 1000);
const int BACK_LIGHT = 13;
LiquidCrystal lcd(12, 11, 0, 1, 9, 8);

void setup()
{
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  pinMode(15, OUTPUT);
  pinMode(BACK_LIGHT, OUTPUT);
  digitalWrite(BACK_LIGHT, HIGH);
  lcd.begin(16, 2);
  lcd.clear();


}

void loop()
{
  alarmState = EEPROM.read(0);

  if (alarmState == 1)
  {
    boolean iS = LOW;
    iS = checkSensors();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WELCOME");
    lcd.setCursor(0, 1);
    lcd.print("ALARM ENGAGED");
    delay(dl);

    if (iS == HIGH)
    {
      delay(100);
      iS = checkSensors();
      if (iS == HIGH)
        Shout();
    }
  }

  else
  {
    buzzer(0);
    char cK = 'E';
    cK = getKey();
    lcd.clear();
    lcd.setCursor(0, 0);
    int i = 1;
    lcd.print("TIJORI 1.0");

    lcd.setCursor(0, 1);
    lcd.print("ALARM OFF");
    delay(dl);

    if (cK == 'B' || cK == 'C')
    {

      unsigned long pm = millis();
      while (cK == 'B')
      {
        byte rP = 2;
        rP = enterPass();
        if (rP == 1)
          changePass();
        else if (rP == 0)
        {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("WRONG PASSWORD");
          lcd.setCursor(0, 1);
          lcd.print("TRY AGAIN !!!");
          delay(1000);
          break;
        }
        else if (millis() - pm > 15000)
          break;
      }
      while (cK == 'C')
      {
        byte rP = 2;
        rP = enterPass();
        if (rP == 1)
          Engage();
        else if (rP == 0)
        {
          break;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("WRONG PASSWORD");
          lcd.setCursor(0, 1);
          lcd.print("TRY AGAIN !!!");

          delay(1000);
        }
        if (millis() - pm > 15000)
          break;
      }
    }
  }
}

void Engage()
{
  for (int i = 25; i > 0; i--)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ACTIVE IN");
    lcd.print(i);
    lcd.print("s");
    lcd.setCursor(0, 1);
    lcd.print("TIJORI 1.0");
    delay(1000);
  }
  s1r = Sen1();
  s2r = Sen2();
  EEPROM.write(0, 1);
}

void changePass()
{
  //Later
}


void Shout()
{
  lcd.clear();

  byte c = 0;
  unsigned long pm = millis();
  while (1)
  {
    lcd.setCursor(0, 0);
    lcd.print("ENTER PASSWORD");

    byte rP = 2;
    rP = enterPass();
    if (rP == 1)
    {
      buzzer(0);
      EEPROM.write(0, 0);
      break;
    }
    else if (rP == 0)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("TRY AGAIN");
      lcd.setCursor(0, 1);
      lcd.print("ATTEMPT LEFT=");
      int k = 4 - c;
      lcd.print(k);

      delay(1000);
      c++;
      if (c == 5)
        buzzer(1);
    }
    else if (millis() - pm > 15000)
      buzzer(1);
  }
}


char getKey()
{
  int ret = -1;
  if (analogRead(A0) == 0)
  {
    delay(50);
    if (analogRead(A0) == 0)
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

byte enterPass()
{
  lcd.clear();
  lcd.print("ENTER PASSWORD");
  for (int i = 0; i <= index; i++)
  {
    if(i>0)
    {
    lcd.setCursor(i-1, 1);
    lcd.print("*");
    delay(dl);
    }
  }

  char ch = getKey();
  if (ch >= '0' && ch <= '9')
    entPass = (entPass * 10) + (ch - '0');
  else if (ch == 'D')
  {

    if (entPass == pass)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("PASSWORD CORRECT");
      lcd.setCursor(0, 1);
      lcd.print("SYS DEACTIVATED");

      delay(1000);
      return (1);
      entPass = 0;
    }

    else
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("PASSWORD INCORRECT");
      lcd.setCursor(0, 1);
      lcd.print("ALARM ON");
      return (0);
    }

  }
  return (2);
}


void buzzer(byte bS)
{
  if (bS == 0)
    digitalWrite(15, LOW);
  else if (bS == 1)
  {
    digitalWrite(15, HIGH);
    delay(100);
    digitalWrite(15, LOW);
    delay(50);
  }
}


int Sen1() {
  delay(50);
  unsigned int uS = sonar.ping();
  uS = uS / US_ROUNDTRIP_CM;
  return (uS);
}

int Sen2() {
  delay(50);
  unsigned int uS2 = sonar2.ping();
  uS2 = uS2 / US_ROUNDTRIP_CM;
  return (uS2);
}

boolean checkSensors()
{
  int s1 = Sen1();
  int s2 = Sen2();
  boolean s3 = digitalRead(5);
  boolean s4 = digitalRead(6);
  boolean s5 = digitalRead(7);
  boolean senRead[] = {0, 0, 0, 0, 0};

  if (s1 < s1r) senRead[0] = 1; else senRead[0] = 0;
  if (s2 < s2r) senRead[1] = 1; else senRead[1] = 0;
  if (s3 == 1)  senRead[2] = 1; else senRead[2] = 0;
  if (s4 == 1)  senRead[3] = 1; else senRead[3] = 0;
  if (s4 == 1)  senRead[3] = 1; else senRead[3] = 0;
  if (s5 == 1)  senRead[4] = 1; else senRead[4] = 0;

  if (senRead[0] == 1 && senRead[2] == 1)    return (1);
  else if (senRead[1] == 1 && senRead[3] == 1)    return (1);
  else if (senRead[4] == 1)    return (1);
  else    return (0);
}

