#include <Servo.h>
#include <NewPing.h>

#define TRIG_PIN        12
#define ECHO_PIN        13
#define MAX_DISTANCE_CM 200

#define SENSOR_LEFT_PIN  A2
#define SENSOR_RIGHT_PIN A5

#define ENA 5
#define IN1 3
#define IN2 4
#define ENB 6
#define IN3 2
#define IN4 7

#define FORWARD_ANGLE 90
#define LEFT_ANGLE    120 
#define RIGHT_ANGLE   60   

#define DISTANCE_THRESHOLD_CM 35

Servo ultrasonicServo;
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE_CM);
int lastServoAngle = FORWARD_ANGLE;

void setup() {
  Serial.begin(9600);

  pinMode(SENSOR_LEFT_PIN, INPUT);
  pinMode(SENSOR_RIGHT_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  ultrasonicServo.attach(A0, 700, 2400);
  ultrasonicServo.write(FORWARD_ANGLE);
}

long getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long cm = sonar.ping_cm();
  return cm == 0 ? MAX_DISTANCE_CM + 1 : cm;
}

void forward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 140);
  analogWrite(ENB, 140);
}

void softRight() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 160);
}

void softLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 160);
  analogWrite(ENB, 0);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void smoothServoMove(int fromAngle, int toAngle, int stepDelay = 10) {
  if (fromAngle < toAngle) {
    for (int a = fromAngle; a <= toAngle; a++) {
      ultrasonicServo.write(a);
      delay(stepDelay);
    }
  } else {
    for (int a = fromAngle; a >= toAngle; a--) {
      ultrasonicServo.write(a);
      delay(stepDelay);
    }
  }
}

void loop() {
  int sensorLeft  = digitalRead(SENSOR_LEFT_PIN);
  int sensorRight = digitalRead(SENSOR_RIGHT_PIN);
  long distanceCm = getDistance();

  bool seesLeft   = (sensorLeft  == LOW);
  bool seesRight  = (sensorRight == LOW);

  int targetAngle = lastServoAngle;

  // Ако и двата IR засичат, движи напред и насочи серво напред
  if (seesLeft && seesRight) {
    forward();
    targetAngle = FORWARD_ANGLE;
  }
  // Само десен IR засича → завиване надясно и насочване на серво надясно
  else if (seesRight) {
    softRight();
    targetAngle = RIGHT_ANGLE;
  }
  // Само ляв IR засича → завиване наляво и насочване на серво наляво
  else if (seesLeft) {
    softLeft();
    targetAngle = LEFT_ANGLE;
  }
  // Никакво засичане → спиране, серво остава насочено към последната посока
  else {
    stopMotors();
  }

  // Плавно движение на серво само при промяна
  if (targetAngle != lastServoAngle) {
    smoothServoMove(lastServoAngle, targetAngle);
    lastServoAngle = targetAngle;
  }

  Serial.print("L IR: "); Serial.print(sensorLeft);
  Serial.print(" R IR: "); Serial.print(sensorRight);
  Serial.print(" Dist: "); Serial.print(distanceCm);
  Serial.print(" Angle: "); Serial.println(lastServoAngle);

  delay(30);
}