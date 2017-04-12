void setup() {
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, INPUT);
  digitalWrite(6, HIGH);
}

void loop() {
  int SL = analogRead(A1);
  int SR = analogRead(A2);
  boolean ST = digitalRead(6);
  if (ST == HIGH)
  {
    if (SL > SR + 50)
    {
      digitalWrite(4, HIGH);
      digitalWrite(5, LOW);
    }
    else if (SR > SL + 50)
    {
      digitalWrite(5, HIGH);
      digitalWrite(4, LOW);
    }
    else
    {
      digitalWrite(5, LOW);
      digitalWrite(4, LOW);
    }
  }
  else
  {
    digitalWrite(5, LOW);
    digitalWrite(4, LOW);
  }
}
