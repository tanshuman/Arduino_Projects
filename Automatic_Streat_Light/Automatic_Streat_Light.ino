int sv[5], rv[5], sum;
const int ol[5] = {11, 10, 9, 5, 6};
const int it = 400;
unsigned long cm, diff[5];
unsigned long pm[5] = {0, 0, 0, 0, 0};
int sl[5] = {0, 0, 0, 0, 0};

void setup() {
  Serial.begin(9600);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
}

void loop() {
  digitalWrite(12, LOW);
  for (int i = 0; i < 5; i++)
    analogWrite(ol[i], 0);
  sum = 0;
  sv[0] = analogRead(14);
  if (analogRead(14) < it) {
    for (int i = 0; i < 5; i++)
      analogWrite(ol[i], 10);
    for (int i = 1; i < 5; i++) {
      int j = i + 14;
      rv[i] = analogRead(j) / 4;
      Serial.println(analogRead(j));
      sum = sum + rv[i];
    }
    digitalWrite(12, HIGH);
    sum = sum - 200;
    Serial.print("Reference Value = ");
    Serial.println(sum);
    
    while (analogRead(14) < it)
    {

      for (int i = 0; i < 5; i++)
        sl[i] = 0;

      cm = millis();

      for (int i = 1; i < 5; i++)
      {
        int j = i + 14;
        sv[i] = analogRead(j);
      }

      for (int i = 1; i < 5; i++)
      {
        diff[i] = cm - pm[i];

        if (sv[i] < sum || diff[i] < 5000)  {
          assign(i);
          if (sv[i] < sum)
            pm[i] = millis();
        }
      }

      for (int i = 0; i < 5; i++)
      {
        if (sl[i] == 1)
          analogWrite(ol[i], 100);
        else
          analogWrite(ol[i], 10);
      }
    }
  }
}


void assign(int m)
{
  int k = m + 1;
  int j = m - 1;
  for (j; j < k; j++)
    sl[j] = 1;
}

