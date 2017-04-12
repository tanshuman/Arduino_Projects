int xpin = A0;
int ypin = A1;
void setup()
{
  Serial.begin(9600); //To start serial communication
}

void loop()
{
  int x = analogRead(xpin);   //to read adv value of x pin and save it to variable x
  int y = analogRead(ypin);   //to read adv value of y pin and save it to variable y
  Serial.print(x);             //print the value of x axis
  Serial.print("\t");         //print tab
  Serial.println(y);             //print the value of y axis
  delay(500);
}

