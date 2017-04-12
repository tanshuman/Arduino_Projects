char directions[30];
char reshortn[30];
char shortn[30];
#include<LiquidCrystal.h>
LiquidCrystal lcd(12,11,5,4,3,2);
int rm1=0; //naming motors pins
int rm2=1;
int lm1=24;
int lm2=25;
int l,c1,c2,c3,r;
int i=0;
int k=0;
int tdelay=750;    //delay for turn
int fdelay=500;    //delay for forward

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

int eosens() //to check the values of middle sensors for special cases
{
  readsens();
  if(((c1+c2+c3)==1) || ((c1+c2+c3)==2))
    return 1;
  else
    return 0;
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

void inch()  //perform inch function
{
  lcd.print("Inch Function");
  Stop();    //stop the robot
  delay(100);
  forward();  //move forward
  delay(500);
//  Stop();
//  delay(100);
  readsens();  //read the values of sensors
  delay(100);
  lcd.clear();
}

void align() //to align the robot
{
  lcd.clear();
  lcd.print("ALIGN");
  Stop();    //stop the robot
  delay(1000);
  forward();  //move forward
  delay(60);
  lcd.clear();  //clear lcd
  readsens();  //read the values of sensors
}

void printing(char prtdirection[])  //function to print direction on robot
{
  lcd.clear();
  for(i=0; prtdirection[i]!='E';i++)
    lcd.print(prtdirection[i]);
  delay(2000);
}

void setup()
{
  lcd.begin(16,2);  //initializing LCD
  lcd.print("MSR with LHR");
  delay(500);
  lcd.clear();    //clear LCD
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
  lcd.clear(); //clear the lcd
  readsens();  //read the values of sensors
  lcd.clear();  //clear the lcd
  
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
    directions[i]='U';    //store U in the direction array
    i++;        //increament the position
  }
  
  //----------------------------------MAZE CASES---------------------------//
  else if(((l==0)&&(c1==0))||((c3==0)&&(r==0)))
  {
    align();    //align the robot
    
    //Right only and str and right
    if(((l==1)&&(c1==0)&&(c2==0)&&(c3==0)&&(r==0)) || ((l==1)&&(c1==1)&&(c2==0)&&(c3==0)&&(r==0)))
    {
      lcd.print("RT/Strt n rt?");
      inch();  //inch function
      if((l==1)&&(c1==1)&&(c2==1)&&(c3==1)&&(r==1))
      {
        lcd.print("right");
        right();  //turn right
        delay(tdelay);    //turn delay
      }
      else if((l==1)&&(r==1)&&(eosens()))
      {
        lcd.print("Stright");
        directions[i]='S';    //store 'S' in the array
        i++;    //increment to the next position in the array
        forward();  //forward
        delay(fdelay);  //forward delay
      }
    }
    //Left only and St & Left
    else if(((l==0)&&(c1==0)&&(c2==0)&&(c3==0)&&(r==1)) || ((l==0)&&(c1==0)&&(c2==0)&&(c3==1)&&(r==1)))
    {
      lcd.print("LFT/Strt n Lft?");
      inch();  //inch function
      if((l==1)&&(c1==1)&&(c2==1)&&(c3==1)&&(r==1))
      {
        lcd.print("left");
        left();  //turn left
        delay(tdelay);    //turn delay
      }
      else if((l==1)&&(r==1)&&(eosens()))
      {
        lcd.print("left");
        directions[i]='L';    //store 'L' in the array
        i++;    //increment to the next position in the array
        left();  //left
        delay(tdelay);  //turn delay
      }
    }
    //4-Way / T-tint/ End of MAze
    else if((l==0)&&(c1==0)&&(c2==0)&&(c3==0)&&(r==0))
    {
      lcd.print("T/END/4");  
      inch() ;  //inch function
      if((l==1)&&(r==1)&&(eosens()))
      {
        lcd.print("4 Way");
        directions[i]='L';  //store 'L' into the array
        i++;
        left();  //turn left
        delay(tdelay);  //turn delay
      }
      else if((l==1)&&(c1==1)&&(c2==1)&&(c3==1)&&(r==1))
      {
        lcd.print("T-int");
        directions[i]='L';  //store 'L' into the array
        i++;
        left();  //turn left
        delay(tdelay);  //turn delay
      }
      else if((l==0)&&(c1==0)&&(c2==0)&&(c3==0)&&(r==0))
      {
        lcd.print("END OF MAZE");
        Stop();  //stop the robot
        directions[i]='E';  //store 'E' into the array
        printing(directions);  //print stored directions
        delay(10000);  //10 Sec Delay
        lcd.clear();
        replacement(directions);
      }
    }
  }
}

void final()
{
  lcd.clear();  //clear the lcd
  lcd.print("Entered Final");
  delay(1000);
  while(1)
  {
    lcd.clear();
    readsens();  //read the values of sensors
    lcd.clear();
    
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
  
  //----------------------------------MAZE CASES---------------------------//
  else if(((l==0)&&(c1==0))||((c3==0)&&(r==0)))
  {
    align();    //align the robot
    
    //Right only and str and right
    if(((l==1)&&(c1==0)&&(c2==0)&&(c3==0)&&(r==0)) || ((l==1)&&(c1==1)&&(c2==0)&&(c3==0)&&(r==0)))
    {
      lcd.print("RT/Strt n rt?");
      inch();  //inch function
      if((l==1)&&(c1==1)&&(c2==1)&&(c3==1)&&(r==1))
      {
        lcd.print("right");
        right();  //turn right
        delay(tdelay);    //turn delay
      }
      else if((l==1)&&(r==1)&&(eosens()))
      {
        lcd.print("St. and Right");
        decisions();  //check what decision to take
      }
    }
    //Left only and St & Left
    else if(((l==0)&&(c1==0)&&(c2==0)&&(c3==0)&&(r==1)) || ((l==0)&&(c1==0)&&(c2==0)&&(c3==1)&&(r==1)))
    {
      lcd.print("LFT/Strt n Lft?");
      inch();  //inch function
      if((l==1)&&(c1==1)&&(c2==1)&&(c3==1)&&(r==1))
      {
        lcd.print("left");
        left();  //turn left
        delay(tdelay);    //turn delay
      }
      else if((l==1)&&(r==1)&&(eosens()))
      {
        lcd.print("st & left");
        decisions();  //check what decision to take
      }
    }
    //4-Way / T-tint/ End of MAze
    else if((l==0)&&(c1==0)&&(c2==0)&&(c3==0)&&(r==0))
    {
      lcd.print("T/END/4");  
      inch() ;  //inch function
      if((l==1)&&(r==1)&&(eosens()))
      {
        lcd.print("4 Way");
        decisions();  //check what decision to take
      }
      else if((l==1)&&(c1==1)&&(c2==1)&&(c3==1)&&(r==1))
      {
        lcd.print("T-int");
        decisions();  //check what decision to take
      }
      else if((l==0)&&(c1==0)&&(c2==0)&&(c3==0)&&(r==0))
      {
        lcd.print("END OF MAZE");
        Stop();  //stop the robot
        while(1);
      }
    }
  }
  }
}

void decisions()
{
  Stop();
  lcd.print("Entered Decisions");
  delay(200);
  lcd.clear();
  if(reshortn[k]=='S')  //If element is 'S'
  {
    forward();   //Forward
    delay(fdelay);    //fdelay
  }
  else if(reshortn[k]=='L')  //If element is 'L'
  {
    left();   //Left
    delay(tdelay);    //tdelay
  }
  else if(reshortn[k]=='R')  //If element is 'R'
  {
    right();   //Right
    delay(tdelay);    //tdelay
  }
  k=k++;    //increase position in array
}

void replacement(char shortn[])   // Replacement function
{
lcd.print("ENTERED REPLACEMENT");
delay(1000);
lcd.clear();                     // Clear Lcd
for(i=0;shortn[i]!='E';i++)      // Read all the element in array
{
 if(shortn[i]=='U')              // Check if the element is 'U'
 {
  if((shortn[i-1]=='S')&&(shortn[i+1]=='L')) // SUL is replaced by xxR
  {
   shortn[i-1]='X';
   shortn[i]='X';
   shortn[i+1]='R';
  }
   else if((shortn[i-1]=='L')&&(shortn[i+1]=='L')) // LUL is replaced by xxS
  {
   shortn[i-1]='X';
   shortn[i]='X';
   shortn[i+1]='S';
  }
  else if((shortn[i-1]=='L')&&(shortn[i+1]=='S')) // SUL is replaced by xxR
  {
   shortn[i-1]='X';
   shortn[i]='X';
   shortn[i+1]='R';
  }
   else if((shortn[i-1]=='L')&&(shortn[i+1]=='R')) // LUR is replaced by xxU
  {
   shortn[i-1]='X';
   shortn[i]='X';
   shortn[i+1]='U';
  }
    else if((shortn[i-1]=='R')&&(shortn[i+1]=='L')) //RUL is replaced by xxU
  {
   shortn[i-1]='X';
   shortn[i]='X';
   shortn[i+1]='U';
  }
    else if((shortn[i-1]=='S')&&(shortn[i+1]=='S')) // SUS is replaced by xxU
  {
   shortn[i-1]='X';
   shortn[i]='X';
   shortn[i+1]='U';
  }
 }
}
lcd.clear();     // clear LCD
lcd.print("Finished New Path");
delay(1000);
lcd.print("NEW");
int j=0;
for(i=0; shortn[i]!='E';i++)  // Read every element in the array
 {
  if( shortn[i]!='X')         // Check is the element is not 'X'
  {
    reshortn[j]= shortn[i];   // If it is not 'X' Copy it to the new array
    j++;
  }
 }
 reshortn[j]='E';
 lcd.print("SPA=");
 printing(reshortn);            // Print the path in the LCD
 delay(1000);
 for(i=0; reshortn[i]!='E';i++)  // Read every element in the array
 {
  if( reshortn[i]=='U')         // Check is the element is 'U'
  {
    replacement(reshortn);      // Implement the replacement rules again
  }
 }
 lcd.clear();                 // Clear lcd
 lcd.print("FINAL=");
 printing(reshortn);          //Print the shortest path
 delay(1000);
 final();                     // call the final path 
}
