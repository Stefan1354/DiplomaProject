#include <Servo.h>
#include <NewPing.h>

// Sensor pins
#define TRIG_PIN 12
#define ECHO_PIN 13
#define ENA 5
#define ENB 6
#define IN1 3
#define IN2 4
#define IN3 2
#define IN4 7
#define carSpeed 130
#define maximum_distance 200

// Additional front sensors
int Sensor1 = A2;
int Sensor2 = A5;
#define SENSOR_THRESHOLD 850

boolean goesForward = false;
int distance = 200;
NewPing sonar(TRIG_PIN, ECHO_PIN, maximum_distance);
Servo servo_motor;

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(Sensor1, INPUT);
  pinMode(Sensor2, INPUT);

  servo_motor.attach(A0, 700, 2400);
  servo_motor.write(80);
  delay(500); // По-кратко изчакване
  distance = readPing();
}

void loop() {
  distance = readPing(); // Прочитане на разстояние в началото

  int distanceRight = 0;
  int distanceLeft = 0;
  delay(50);

  // Read front sensors
  int sensor1Value = analogRead(Sensor1);
  int sensor2Value = analogRead(Sensor2);

  if (distance <= 20 || sensor1Value < SENSOR_THRESHOLD || sensor2Value < SENSOR_THRESHOLD) {
    moveStop();
    delay(300);
    moveBackward();
    delay(400);
    moveStop();
    delay(300);
    distanceRight = scanAndRotate(1); // Сканиране надясно
    delay(300);
    distanceLeft = scanAndRotate(-1); // Сканиране наляво
    delay(300);

    if (distanceRight >= distanceLeft) {
      turnRight();
    } else {
      turnLeft();
    }
    moveStop();
  } else {
    moveForward();
  }
}

int scanAndRotate(int direction) { // direction: -1 за ляво, 1 за дясно
  unsigned long startTime = millis();
  int maxDistance = 0;
  while (millis() - startTime < 500) { // 500ms сканиране
    servo_motor.write(90 + direction * 50); // завъртане на сервомотора
    int currentDistance = readPing();
    if (currentDistance > maxDistance) {
      maxDistance = currentDistance;
    }
    if (direction == 1) {
      turnRight();
    } else {
      turnLeft();
    }
  }
  servo_motor.write(80); // връщане на сервомотора в център
  return maxDistance;
}

int readPing() {
  delay(70);
  int cm = sonar.ping_cm();
  if (cm == 0) {
    cm = 250;
  }
  return cm;
}

void moveStop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void moveForward() {
  if (!goesForward) {
    goesForward = true;
    analogWrite(ENA, carSpeed);
    analogWrite(ENB, carSpeed);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  }
}

void moveBackward() {
  goesForward = false;
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  delay(500);
}

void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  delay(500);
}