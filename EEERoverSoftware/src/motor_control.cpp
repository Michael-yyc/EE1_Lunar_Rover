#include <Arduino.h>
#include <motor_control.h>

//Motor left pins
const int leftPin_en = 8;
const int leftPin_dir = 9;

// Motor right pins
const int rightPin_en = 11;
const int rightPin_dir = 12;


//move forward
void forward()
{
  analogWrite(rightPin_en, 255);
  digitalWrite(rightPin_dir, LOW);
  analogWrite(leftPin_en, 255);
  digitalWrite(leftPin_dir, LOW);
}

//move backward
void backward()
{
    analogWrite(rightPin_en, 255);
    digitalWrite(rightPin_dir, HIGH);
    analogWrite(leftPin_en, 255);
    digitalWrite(leftPin_dir, HIGH);
}

//move right
void right()
{
  analogWrite(rightPin_en, 255);
  digitalWrite(rightPin_dir, HIGH);
  analogWrite(leftPin_en, 255);
  digitalWrite(leftPin_dir, LOW);
}

//move left
void left()
{
  analogWrite(rightPin_en, 255);
  digitalWrite(rightPin_dir, LOW);
  analogWrite(leftPin_en, 255);
  digitalWrite(leftPin_dir, HIGH);
}

//stop
void stop() 
{
  analogWrite(rightPin_en, 0);
  digitalWrite(rightPin_dir, LOW);
  analogWrite(leftPin_en, 0);
  digitalWrite(leftPin_dir, LOW);
}