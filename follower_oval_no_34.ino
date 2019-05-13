
#include <Wire.h>
#define uchar unsigned char
uchar t;
uchar data[16];

//white data > 100
//black data < 100

int leftfront = 11;
int leftback = 10;
int rightfront = 6;
int rightback = 5;
int leftspeedpin = 3;
int rightspeedpin = 9;

int trigPin = 13; // variables for ultra
int echoPin = 12;
long duration;
long distance;

void setup()
{
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
  t = 0;
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}
void loop()
{
  ultra();
  receivedata();
  linefollower();
  prints();
  slowdown();
}

void slowdown() {
  if (distance <= 8) {
    drive(0, 0);
    delay(200);
  }
  if ((distance > 8) && (distance < 18)) {
    linefollower2();
    delay(200);
  }
  else if (distance > 18) {
    linefollower();
  }

}

void linefollower() {
  if (data[14] < 100 && data[0] > 100) { //if outside on black
    drive(150, 150);
  }
  if (data[12] < 100 && data[0] > 100) { //if outside on black
    drive(-160, 190);
  }
  if (data[10] < 100 && data[0] > 100) { //if outside on black
    drive(-160, 190);
  }
    if (data[8] < 100 && data[0] > 100) { //if outside on black
    drive(-160, 190);
  }
  else if (data[14] > 100 && data[2] > 100) { //if on white
    drive(150, 100);
  }
  else if (data[12] < 100 && data[2] < 100) { //if both black
    drive(0, 0);
    delay(120);
  }
}

void drive (int leftspeed, int rightspeed) {
  if (leftspeed > 0) {
    digitalWrite(leftback, LOW); //turned off
    digitalWrite(leftfront, HIGH); //turned on
  }
  else if (leftspeed < 0) {
    digitalWrite(leftback, HIGH);
    digitalWrite(leftfront, LOW);
  }
  else if (leftspeed = 0) {
    digitalWrite(leftback, LOW);
    digitalWrite(leftfront, LOW);
  }
  if (rightspeed > 0) {
    digitalWrite(rightback, LOW);
    digitalWrite(rightfront, HIGH);
  }
  else if (rightspeed < 0) {
    digitalWrite(rightback, HIGH);
    digitalWrite(rightfront, LOW);
  }
  else if (rightspeed = 0) {
    digitalWrite(rightback, LOW);
    digitalWrite(rightfront, LOW);
  }
  analogWrite(leftspeedpin, abs(leftspeed));
  analogWrite(rightspeedpin, abs(rightspeed));
}

void receivedata() {
  Wire.requestFrom(9, 16);    // request 16 bytes from slave device #9
  while (Wire.available())   // slave may send less than requested
  {
    data[t] = Wire.read(); // receive a byte as character
    if (t < 15)
      t++;
    else
      t = 0;
  }
}

void ultra() {
  digitalWrite(trigPin, LOW); //low = off and high = on
  delayMicroseconds(2); // delay
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); //delay
  digitalWrite(trigPin, LOW);  // internal clock begins
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2; // v = d/t
}

void prints() {
  Serial.print("data[2]:");
  Serial.println(data[2]);
  Serial.print("data[12]:");
  Serial.println(data[12]);
  Serial.print("data[8]:");
  Serial.println(data[8]);
  Serial.print("ultra distance:");
  Serial.print(distance);
  Serial.println();
}

void linefollower2() {

  if (data[14] < 100 && data[0] > 100) { //if outside on black
    drive(-10, 55);
  }
  if (data[12] < 100 && data[0] > 100) { //if outside on black
    drive(-10, 55);
  }
  if (data[10] < 100 && data[0] > 100) { //if outside on black
    drive(-10, 55);
  }
  else if (data[14] > 100 && data[2] > 100) { //if on white
    drive(75, 55);
  }
  else if (data[12] < 100 && data[2] < 100) { //if both black
    drive(0, 0);
    delay(120);
  }
}
