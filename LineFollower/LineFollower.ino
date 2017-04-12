char directions[30];
#include<LiquidCrystal.h>
LiquidCrystal lcd(12,11,5,4,3,2);
int rm1=0; //naming motors pins
int rm2=1;
int lm1=24;
int lm2=25;
int l,c1,c2,c3,r;
int i=0;
int tdelay=700;
int fdelay=500;

void Stop() //stop function
{
  digitalWrite(lm1,0);
  digitalWrite(lm2,0);
  digitalWrite(rm1,0);
  digitalWrite(rm2,0);
}

void forward() //fwd function
{
  digitalWrite(lm1,1);
  digitalWrite(lm2,0);
  digitalWrite(rm1,1);
  digitalWrite(rm2,0);
}

void rht() //small right function
{
  digitalWrite(lm1,1);
  digitalWrite(lm2,0);
  digitalWrite(rm1,0);
  digitalWrite(rm2,0);
}

void lft() //small left function
{
  digitalWrite(lm1,0);
  digitalWrite(lm2,0);
  digitalWrite(rm1,1);
  digitalWrite(rm2,0);
}

void right() //sharp right function
{
  digitalWrite(lm1,1);
  digitalWrite(lm2,0);
  digitalWrite(rm1,0);
  digitalWrite(rm2,1);
}

void left() //sharp left function
{
  digitalWrite(lm1,0);
  digitalWrite(lm2,1);
  digitalWrite(rm1,1);
  digitalWrite(rm2,0);
}

void readsens() //reading the values of sensors
{
  l=digitalRead(26);
  c1=digitalRead(27);
  c2=digitalRead(28);
  c3=digitalRead(29);
  r=digitalRead(30);
  lcd.print(l);    //printing the values of sensors
  lcd.print("--");
  lcd.print(c1);
  lcd.print("--");
  lcd.print(c2);
  lcd.print("--");
  lcd.print(c3);
  lcd.print("--");
  lcd.print(r);
}

void setup()
{
  lcd.begin(16,2);  //initializing LCD
  lcd.print("SIMPLE LFR");
  delay(500);
  pinMode(lm1, OUTPUT);  //declare output pins
  pinMode(lm2, OUTPUT);
  pinMode(rm1, OUTPUT);
  pinMode(rm2, OUTPUT);
  pinMode(26, INPUT);    //declare input pins
  pinMode(27, INPUT);
  pinMode(28, INPUT);
  pinMode(29, INPUT);
  pinMode(30, INPUT);
}

void loop()
{
  readsens();
  //------------------------LINE FOLLOWER-------------------//
  //FORWARD
  if((l==1)&&(c1==1)&&(c2==0)&&(c3==1)&&(r==1))
  {
    lcd.print("Foraward");
    forward();  //move fwd
  }
  //LEFT
  else if(((l==1)&&(c1==0)&&(c2==1)&&(c3==1)&&(r==1)) || ((l==1)&&(c1==0)&&(c2==0)&&(c3==1)&&(r==1)))
  {
    lcd.print("Left");
    lft();  //slight left
  }
  //RIGHT
  else if(((l==1)&&(c1==1)&&(c2==1)&&(c3==0)&&(r==1)) || ((l==1)&&(c1==1)&&(c2==0)&&(c3==0)&&(r==1)))
  {
    lcd.print("Right");
    rht();  //slight right
  }
  //DEAD END
  else if((l==1)&&(c1==1)&&(c2==1)&&(c3==1)&&(r==1))
  {
    lcd.print("U Turn");
    left();  //U Turn
    delay(1500);    //delay for U Turn
  }
  else
  {
    lcd.print("Foraward");
    forward();  //move fwd
  }
}
