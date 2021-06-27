#include "motor.h"
// #include "board.h"


Servo::Servo() {
}

void Servo::attach(int pin, int channel) 
{
    _channel = channel;
    _pin = pin;
    const long pwm_freq = US_TO_S / PW_PERIOD_US;
    ledcSetup(_channel, pwm_freq, MAX_RES_BITS);
    ledcAttachPin(_pin, _channel);
}

Servo::~Servo() {
    detach();
}

void Servo::detach() {
    ledcDetachPin(_pin);
}

void Servo::write(float degrees) {
    degrees = constrain(degrees, MIN_ANGLE, MAX_ANGLE);

    const auto pulseUs = constrain(_angleToUs(degrees), PW_WIDTH_MIN, PW_WIDTH_MAX);

    const auto pulseWidthDuty = _usToDuty(pulseUs);

    ledcWrite(_channel, pulseWidthDuty);
}

float Servo::read() const {
  const int duty = ledcRead(_channel);
  const auto pulseUs = _dutyToUs(duty);
  return _usToAngle(pulseUs);
}


void MotorHandler::setupServos() {

  Serial.println("setup servos");

  this->servos = new Servo[JOINT_COUNT];

  for (byte i = 0; i != JOINT_COUNT; ++i) {

    const auto pin = JOINT_PINS[i];

    Serial.print("setup servo ");
    Serial.print(i);
    Serial.print(" on pin ");
    Serial.println(pin);

    this->servos[i].attach(pin, i);
    this->servos[i].write(150);
  }
  Serial.println();

}

bool MotorHandler::setup() {
  setupServos();
  return true;
}

void MotorHandler::loop() {


  this->angle += 0.0005 * dir;

  if (this->angle >= 160 || this->angle <= 140) {
    this->dir *= -1;
  }

  // Serial.print("servo write ");
  // Serial.println(this->angle);
  for (byte i = 0; i != JOINT_COUNT; ++i) {
    this->servos[i].write(this->angle);
  }
}
