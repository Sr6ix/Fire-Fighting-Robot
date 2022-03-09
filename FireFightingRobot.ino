#include <Servo.h>

Servo leftWheel, rightWheel, flameChecker;

boolean forward;
int distance, distancePin, flamePin, fanPin, distanceThreshold, flameThreshold, flameValues[180] = {}, flameAngle;
const int trigPin = 9;
const int echoPin = 10;
long duration;

void setup(){
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  leftWheel.attach(2);
  rightWheel.attach(3);
  flameChecker.attach(4);
  resetWheels();
  forward = true;
  distanceThreshold = 375;
  flameThreshold = 600;
  flamePin = A1;
  Serial.begin(9600);
}

void loop() {
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  
  if(findFlame()){
    int turn = flameAngle - 90;
    if (turn < 0) {
      turn *= -1;
      resetWheels();
      attachWheels();
      leftWheel.write(180);
      rightWheel.write(106);
      delay(turn * (1 / 1250));
    }
    else if (turn > 0) {
      resetWheels();
      attachWheels();
      leftWheel.write(0);
      rightWheel.write(76);
      delay(turn * (1 / 1250));
    }
  }
  else{
    if(!checkDistance()){
      moveForward();
    }
    else{
      Serial.println("Found wall, turning left");
      turnLeft();
      if (!checkDistance()){
        moveForward();
   }
   else{
     Serial.println("Found another wall, turning right");
     turnRight();
     turnRight();
     moveForward();
      }
    }
  }
}

boolean checkDistance() {
  int distance = analogRead(distancePin);
  Serial.print("Distance: ");
  Serial.println(distance);
  return distance > distanceThreshold;
}

boolean findFlame(){
  boolean found = false;
  flameChecker.write(0);
  delay(1000);
  int biggestValue = flameThreshold;
  for(int i = 0; i < 180; i++) {
    flameChecker.write(i);
    delay(5);
    int flameValue = analogRead(flamePin);
    if (flameValue >= biggestValue) {
      biggestValue = flameValue;
      flameAngle = 1;
      found = true;
      Serial.print("Found flame at value: ");
      Serial.print(biggestValue);
      Serial.print(" at angle: ");
      Serial.println(flameAngle);
    }
  }
  return found;
}

void moveForward(){
  Serial.println("Moving forward");
  for(int i = 0; i < 10; i++){
    if(!checkDistance()){
      leftWheel.write(0);
      rightWheel.write(106);
      delay(500);
    }
    else{
      resetWheels();
      return;
    }
  }
  resetWheels();
}

void resetWheels(){
  leftWheel.write(90);
  rightWheel.write(89);
  delay(1000);
  leftWheel.detach();
  rightWheel.detach();
}

void attachWheels(){
  leftWheel.attach(2);
  rightWheel.attach(3);
}

void turnRight(){
  resetWheels();
  attachWheels();
  leftWheel.write(0);
  rightWheel.write(76);
  delay(1250);
}

void turnLeft(){
  resetWheels();
  attachWheels();
  leftWheel.write(180);
  rightWheel.write(106);
  delay(1250);
}
