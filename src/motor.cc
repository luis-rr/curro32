#include "motor.h"
// #include "board.h"

Servo::Servo(int pin, int channel, bool flipped)
: _flipped(flipped), _pin(pin), _channel(channel)
{
    const long pwm_freq = US_TO_S / PW_PERIOD_US;

    ledcAttachPin(_pin, _channel);
    ledcSetup(_channel, pwm_freq, MAX_RES_BITS);
}

Servo::~Servo() {
    ledcWrite(_channel, 0);
    ledcDetachPin(_pin);
}

float mapFloat(float x, float in_min, float in_max, float out_min, float out_max) {
  const auto clipped = constrain(x, in_min, in_max);
  const auto norm = (clipped - in_min) / (in_max - in_min);
  return norm * (out_max - out_min) + out_min;
}

void Servo::_writeTarget(float degrees) {
    const float pulseUs = mapFloat(degrees, MIN_ANGLE, MAX_ANGLE, PW_WIDTH_MIN, PW_WIDTH_MAX);
    const auto pulseWidthDuty = static_cast<uint32_t>(mapFloat(pulseUs, 0, PW_PERIOD_US, 0, DUTY_MAX));

    ledcWrite(_channel, pulseWidthDuty);
}

bool MotorHandler::setup() {
  this->body = new Body();

  this->body->neck.write(0);

  this->body->frsh.write(-30);
  this->body->flsh.write(-30);

  this->body->frkn.write(-45);
  this->body->flkn.write(-45);

  this->body->blsh.write(30);
  this->body->brsh.write(30);

  this->body->blkn.write(45);
  this->body->brkn.write(45);

  angle = 0;

  return true;
}

void MotorHandler::loop() {

  this->angle += 0.0025 * dir;

  if (this->angle <= -60 || 60 <= this->angle) {
    this->dir *= -1;
  }

  this->body->frsh.write(angle -30);
  this->body->frkn.write(-angle -45);

  this->body->flsh.write(angle -30);
  this->body->flkn.write(-angle -45);

  this->body->blsh.write(angle + 30);
  this->body->blkn.write(-angle + 45);

  this->body->brsh.write(angle + 30);
  this->body->brkn.write(-angle + 45);
}
