#include <Arduino.h>
#include <Bluepad32.h>

ControllerPtr controller = nullptr;

// Your actual wiring
const int LEFT_DIR  = 14;
const int LEFT_EN   = 12;

const int RIGHT_DIR = 26;
const int RIGHT_EN  = 27;

// PWM setup
const int LEFT_PWM_CHANNEL  = 0;
const int RIGHT_PWM_CHANNEL = 1;
const int PWM_FREQ = 20000;
const int PWM_RES = 8;
const int PWM_MAX = 255;

// Controller settings
const int DEADZONE_STEER = 60;
const int TRIGGER_DEADZONE = 30;

// Change these if motors spin wrong way
const bool LEFT_FORWARD_DIR  = HIGH;
const bool RIGHT_FORWARD_DIR = HIGH;

unsigned long lastPrint = 0;

void stopMotors();
void driveMotor(int dirPin, int pwmChannel, int speedValue, bool forwardDir);
void driveRover(int leftSpeed, int rightSpeed);

void onConnectedController(ControllerPtr ctl) {
  Serial.println("Controller connected");
  controller = ctl;
}

void onDisconnectedController(ControllerPtr ctl) {
  Serial.println("Controller disconnected");

  if (controller == ctl) {
    controller = nullptr;
  }

  stopMotors();
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(LEFT_DIR, OUTPUT);
  pinMode(RIGHT_DIR, OUTPUT);

  // Attach PWM channels to EN pins
  ledcSetup(LEFT_PWM_CHANNEL, PWM_FREQ, PWM_RES);
  ledcSetup(RIGHT_PWM_CHANNEL, PWM_FREQ, PWM_RES);

  ledcAttachPin(LEFT_EN, LEFT_PWM_CHANNEL);
  ledcAttachPin(RIGHT_EN, RIGHT_PWM_CHANNEL);

  stopMotors();

  BP32.setup(&onConnectedController, &onDisconnectedController);

  // Keep this commented unless repairing pairing.
  // BP32.forgetBluetoothKeys();

  Serial.println("Ready. Pair controller.");
  Serial.println("Left:  DIR D14, EN D12");
  Serial.println("Right: DIR D26, EN D27");
}

void loop() {
  BP32.update();

  if (controller && controller->isConnected()) {
    int steering = -controller->axisX();
    int r2 = controller->throttle();
    int l2 = controller->brake();

    if (abs(steering) < DEADZONE_STEER) steering = 0;
    if (r2 < TRIGGER_DEADZONE) r2 = 0;
    if (l2 < TRIGGER_DEADZONE) l2 = 0;

    int forwardPWM = map(r2, 0, 1023, 0, PWM_MAX);
    int reversePWM = map(l2, 0, 1023, 0, PWM_MAX);

    forwardPWM = constrain(forwardPWM, 0, PWM_MAX);
    reversePWM = constrain(reversePWM, 0, PWM_MAX);

    // R2 forward, L2 reverse
    int driveSpeed = forwardPWM - reversePWM;

    int steeringAmount = map(steering, -512, 512, -PWM_MAX, PWM_MAX);
    steeringAmount = constrain(steeringAmount, -PWM_MAX, PWM_MAX);

    int leftSpeed = driveSpeed;
    int rightSpeed = driveSpeed;

    int turnStrength = abs(steeringAmount) * abs(driveSpeed) / PWM_MAX;

    if (steeringAmount > 0) {
      // Joystick right: slow right motor
      if (driveSpeed > 0) {
        rightSpeed = driveSpeed - turnStrength;
      } else if (driveSpeed < 0) {
        rightSpeed = driveSpeed + turnStrength;
      }
    } 
    else if (steeringAmount < 0) {
      // Joystick left: slow left motor
      if (driveSpeed > 0) {
        leftSpeed = driveSpeed - turnStrength;
      } else if (driveSpeed < 0) {
        leftSpeed = driveSpeed + turnStrength;
      }
    }

    leftSpeed = constrain(leftSpeed, -PWM_MAX, PWM_MAX);
    rightSpeed = constrain(rightSpeed, -PWM_MAX, PWM_MAX);

    // Important safety stop
    if (r2 == 0 && l2 == 0) {
      leftSpeed = 0;
      rightSpeed = 0;
    }

    driveRover(leftSpeed, rightSpeed);

    if (millis() - lastPrint > 200) {
      Serial.print("R2: ");
      Serial.print(r2);
      Serial.print(" | L2: ");
      Serial.print(l2);
      Serial.print(" | Steering: ");
      Serial.print(steering);
      Serial.print(" | Left PWM: ");
      Serial.print(abs(leftSpeed));
      Serial.print(" | Right PWM: ");
      Serial.print(abs(rightSpeed));
      Serial.print(" | Left DIR: ");
      Serial.print(leftSpeed >= 0 ? "FWD" : "REV");
      Serial.print(" | Right DIR: ");
      Serial.println(rightSpeed >= 0 ? "FWD" : "REV");

      lastPrint = millis();
    }
  } 
  else {
    stopMotors();

    if (millis() - lastPrint > 1000) {
      Serial.println("Controller not connected. Motors stopped.");
      lastPrint = millis();
    }
  }

  delay(20);
}

void driveRover(int leftSpeed, int rightSpeed) {
  driveMotor(LEFT_DIR, LEFT_PWM_CHANNEL, leftSpeed, LEFT_FORWARD_DIR);
  driveMotor(RIGHT_DIR, RIGHT_PWM_CHANNEL, rightSpeed, RIGHT_FORWARD_DIR);
}

void driveMotor(int dirPin, int pwmChannel, int speedValue, bool forwardDir) {
  speedValue = constrain(speedValue, -PWM_MAX, PWM_MAX);

  if (speedValue > 0) {
    digitalWrite(dirPin, forwardDir);
    ledcWrite(pwmChannel, speedValue);
  } 
  else if (speedValue < 0) {
    digitalWrite(dirPin, !forwardDir);
    ledcWrite(pwmChannel, -speedValue);
  } 
  else {
    ledcWrite(pwmChannel, 0);
  }
}

void stopMotors() {
  ledcWrite(LEFT_PWM_CHANNEL, 0);
  ledcWrite(RIGHT_PWM_CHANNEL, 0);

  digitalWrite(LEFT_DIR, LOW);
  digitalWrite(RIGHT_DIR, LOW);
}