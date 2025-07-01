#define ENA 5
#define ENB 6
#define IN1 3
#define IN2 4
#define IN3 2
#define IN4 7

int Sensor1;
int Sensor2;
int Sensor3;
int Sensor4;

void setup() {

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(8, INPUT);
  pinMode(9, INPUT);
  pinMode(10, INPUT);
  pinMode(11, INPUT);
  Serial.begin(9600);
}

void loop()
{
    //Четем състоянието дали има сигнал (HIGH) или не (LOW)
    Sensor1 = digitalRead(8); //IN1
    Sensor2 = digitalRead(9); //IN2
    Sensor3 = digitalRead(10); //IN3
    Sensor4 = digitalRead(11); //IN4

    Serial.print(Sensor4);
    Serial.print("--");
    Serial.print(Sensor3);
    Serial.print("--");
    Serial.print(Sensor2);
    Serial.print("--");
    Serial.println(Sensor1);


  // Централните сензори хващат линията => напред
  if(Sensor2 ==  HIGH && Sensor3 == HIGH) {
    forward();

  // Линията е по-наляво => по-рязък ляв завой
  } else if(Sensor4 == HIGH && Sensor3 == LOW) {
    left_M();

  // Линията е вляво => ляв завой
  } else if(Sensor4 == HIGH) {
    left();

    // Линията е по-вдясно => по-рязък десен завой
  } else if(Sensor1 == HIGH && Sensor2 == LOW) {
    right_M();

  // Линията е вдясно => десен завой
  } else if(Sensor1 == HIGH) {
    right();

  // Всички на бяло => спри
  } else if(Sensor1 == LOW && Sensor2 == LOW && Sensor3 == LOW && Sensor4 == LOW) {
    stop();

  // За всякакви други ситуации (преходи, загуба на линия) — бавно напред
  } else {
    forward();
  }

  delay(10);
}


void forward() {
  analogWrite(ENA, 110);
  analogWrite(ENB, 110);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  // Serial.println("Forward");
}

void left() {
  analogWrite(ENA, 150);
  analogWrite(ENB, 150);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  // Serial.println("Left");
}

void left_M() {//left function
  analogWrite(ENA, 200);
  analogWrite(ENB, 200);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  // Serial.println("Left");
}

void right() {//right function
  analogWrite(ENA, 150);
  analogWrite(ENB, 150);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  // Serial.println("Right");
}

void right_M() {//right function
  analogWrite(ENA, 200);
  analogWrite(ENB, 200);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  // Serial.println("Right");
}

void stop() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  // Serial.println("Stop!");
}