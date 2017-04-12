/* Gesture Controlled Robot - Receiver Side
   Date : 05/01/2016
   By :   Anshuman @ Lema Labs
   
*/
int ML1 = 0;            //Variable for left motor terminal 1
int ML2 = 1;            //Variable for left motor terminal 2
int MR1 = 24;           //Variable for right motor terminal 1
int MR2 = 25;           //Variable for right motor terminal 2

void setup()
{
  Serial.begin(9600); //To start serial communication
  pinMode(ML1, OUTPUT);    //Setting motors pins as Output
  pinMode(ML2, OUTPUT);    //Setting motors pins as Output
  pinMode(MR1, OUTPUT);    //Setting motors pins as Output
  pinMode(MR2, OUTPUT);    //Setting motors pins as Output
}

void loop()
{
  while(!Serial.available()); //Waiting for Serial Data
  
  char b = Serial.read();     //Reading the Serial Data and Storing it to b
  if(b=='S')                  //else if Data id S
    Stop();                     //Function to stop the motor
  else if(b=='F')
    Fwd();                      //To move robot forward
  else if(b=='B')
    Bck();                      //To move robot backward
  else if(b=='L')
    Left();                     //To move robot sharp left
  else if(b=='R')
    Right();                    //To move robot sharp right
  else if(b=='l')
    Lft();                      //To move robot slight left
  else if(b=='r')
    Rht();    //To move robot slight right
  else
    Stop();
  
}

void Stop()              //Function to stop the robot
{
  digitalWrite(ML1,0);
  digitalWrite(ML2,0);
  digitalWrite(MR1,0);
  digitalWrite(MR2,0);
}

void Fwd()              //Function to move robot forward
{
  digitalWrite(ML1,1);
  digitalWrite(ML2,0);
  digitalWrite(MR1,1);
  digitalWrite(MR2,0);
}

void Bck()              //Function to move robot back
{
  digitalWrite(ML1,0);
  digitalWrite(ML2,1);
  digitalWrite(MR1,0);
  digitalWrite(MR2,1);
}

void Left()              //Function to move robot sharp left
{
  digitalWrite(ML1,0);
  digitalWrite(ML2,1);
  digitalWrite(MR1,1);
  digitalWrite(MR2,0);
}

void Right()              //Function to move robot sharp right
{
  digitalWrite(ML1,1);
  digitalWrite(ML2,0);
  digitalWrite(MR1,0);
  digitalWrite(MR2,1);
}

void Lft()              //Function to move robot slight left
{
  digitalWrite(ML1,0);
  digitalWrite(ML2,0);
  digitalWrite(MR1,1);
  digitalWrite(MR2,0);
}

void Rht()              //Function to move robot slight right
{
  digitalWrite(ML1,1);
  digitalWrite(ML2,0);
  digitalWrite(MR1,0);
  digitalWrite(MR2,0);
}
