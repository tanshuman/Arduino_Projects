/* Gesture Controlled Robot - Receiver Side
   Date : 05/01/2016
   By :   Anshuman @ Lema Labs
   
*/
int ML1 = 0;            //Variable for left motor terminal 1
int ML2 = 1;            //Variable for left motor terminal 2
int MR1 = 2;           //Variable for right motor terminal 1
int MR2 = 3;           //Variable for right motor terminal 2

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
  
}

