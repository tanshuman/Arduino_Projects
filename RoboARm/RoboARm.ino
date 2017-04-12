#include <LiquidCrystal.h>
#include <Servo.h>
#define ML1 0
#define ML2 1
#define MR1 24
#define MR2 25
#define b1 14
#define b2 13
#define b3 15
#define b4 16
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
Servo myservo;
int val=180;
void setup()
{
  // initialize the serial communications:
  Serial.begin(9600);
  // attach servo to 31
  myservo.attach(31);
  myservo.write(val);
  // initialize lcd
   lcd.begin(16, 2);
   lcd.print("ROBOTIC ARM");

  // robot motor
  pinMode(ML1,OUTPUT);
  pinMode(ML2,OUTPUT);
  pinMode(MR1,OUTPUT);
  pinMode(MR2,OUTPUT);
  // robot arm motor
  pinMode(b1,OUTPUT);               
  pinMode(b2,OUTPUT);
  pinMode(b3,OUTPUT);
  pinMode(b4,OUTPUT);
  delay(500);
}

void loop()
{ 
lcd.clear();
while(!Serial.available())
{
  lcd.clear();
  lcd.print("Nothing Coming");
  delay(20);
}
  char getdata = Serial.read();  // if data available read it and move it to getdata
  // if received data is F move forward
  lcd.print(getdata);
  if(getdata == 'W')
    Fwd();
   // if received data is B move reverse
  if(getdata == 'S')
    Bck(); 
   // if received data is L move left
  if(getdata == 'A')
    Left();
   // if received data is R move right
  if(getdata == 'D')
    Right();
   // if received data is U move B1 motor move up
  if(getdata == 'Z')
  {
       digitalWrite(b1,HIGH);
       digitalWrite(b2,LOW);
  }
   // if received data is D move B1 motor move down
  if(getdata == 'X')
  {
      digitalWrite(b1,LOW);
      digitalWrite(b2,HIGH);
  }
   // if received data is Z move B2 motor move up
  if(getdata == 'Z')
  {
      digitalWrite(b3,HIGH);
      digitalWrite(b4,LOW);
  }
   // if received data is X move B2 motor move down
  if(getdata == 'X')
  {
      digitalWrite(b1,LOW);
      digitalWrite(b2,HIGH);
  }
   // if received data is K move griper OPen
  if(getdata == 'K' && val<=180)
   {
     val=val++;
     myservo.write(val);
     delay(15);
   }
   // if received data is L move griper Close
  if(getdata == 'J' && val>=100)
   {
     val=val--;
     myservo.write(val);
     delay(15);
   }
}




void Stop()              //Function to stop the robot
{
  digitalWrite(ML1,0);
  digitalWrite(ML2,0);
  digitalWrite(MR1,0);
  digitalWrite(MR2,0);
  digitalWrite(b1,LOW);
  digitalWrite(b2,LOW);
  digitalWrite(b3,LOW);
  digitalWrite(b4,LOW);

}

void Fwd()              //Function to move robot forward
{
  digitalWrite(ML1,1);
  digitalWrite(ML2,0);
  digitalWrite(MR1,1);
  digitalWrite(MR2,0);
  delay(50);
}

void Bck()              //Function to move robot back
{
  digitalWrite(ML1,0);
  digitalWrite(ML2,1);
  digitalWrite(MR1,0);
  digitalWrite(MR2,1);
  delay(50);
}

void Left()              //Function to move robot sharp left
{
  digitalWrite(ML1,0);
  digitalWrite(ML2,1);
  digitalWrite(MR1,1);
  digitalWrite(MR2,0);
  delay(50);
}

void Right()              //Function to move robot sharp right
{
  digitalWrite(ML1,1);
  digitalWrite(ML2,0);
  digitalWrite(MR1,0);
  digitalWrite(MR2,1);
  delay(50);
}

