#ifndef CURRO_MOTOR_H_
#define CURRO_MOTOR_H_

#include <Arduino.h>


class Servo {
  // Manage a single PS1 servo motor connected to a pin.
  // Note all ESP32 pins are capable of PWM (pulse wave modulation),
  // Instead of Arduino's classic "analogWrite", ESP32 implements "ledcWrite"
  // which is similar (see https://techexplorations.com/guides/esp32/begin/pwm).

  public:

    // PWM wave period
    static const int PW_PERIOD_US = 2500;
    static const int MIN_ANGLE = 0;
    static const int MAX_ANGLE = 290;
    static const int PW_WIDTH_MIN = 500;
    static const int PW_WIDTH_MAX = 2500;
    static const int MAX_RES_BITS = 16;
    static const int DUTY_MAX = (1 << MAX_RES_BITS) - 1;

    const float US_TO_S = 1000 * 1000;

    Servo();
    ~Servo();

     /**
     * @brief Associate this instance with a servomotor whose input is
     *        connected to pin.

     * @param pin Pin connected to the servo pulse wave input. This
     *            pin must be capable of PWM output (all ESP32 pins).
     *
     * @param channel Channel which is set to ESP32 Arduino function ledcSetup().
     *                Channel must be number between 0 - 15.
     *                It is possible to use automatic channel setup with constant
     *                Servo::CHANNEL_NOT_ATTACHED.
     */
    void attach(int pin, int channel);

    void detach();

    // Set the servo's target angle
    void write(float degrees);

    // Get the servo's target angle
    float read() const;

  private:

    static int _usToDuty(int us)    { return map(us, 0, PW_PERIOD_US, 0, DUTY_MAX); }
    static int _dutyToUs(int duty)  { return map(duty, 0, DUTY_MAX, 0, PW_PERIOD_US); }
    static float _usToAngle(int us)   { return map(us, PW_WIDTH_MIN, PW_WIDTH_MAX, MIN_ANGLE, MAX_ANGLE); }
    static float _angleToUs(float angle){ return map(angle, MIN_ANGLE, MAX_ANGLE, PW_WIDTH_MIN, PW_WIDTH_MAX); }

    int _pin;
    int _channel;
};


class MotorHandler {
  void setupServos();

  Servo* servos;
  float angle = 150;
  int dir = 1;

public:
  bool setup();
  void loop();


  /*

    PIN, PW channel:

    --------------------------
    |    4,  9, frkn     2, 10, brkn  |
    |    5,  5, frsh    15,  6, brsh  |
    |    18, 1, head    13,  2, ----  |
    |-----------                      |
    |           |                     |
    |   ESP32   |  IMU          USB-C |
    |           |                     |
    |-----------                      |
    |    32, 0, ----    12,  3, ----  |
    |    33, 4, flsh    14,  7, blsh  |
    |    19, 8, flkn    27, 11, blkn  |
    --------------------------

  */

  const uint8_t JOINT_PINS[12] = {
    // head
    18, 

    // shoulders
    5, 15, 14, 33,

    // knees
    4, 2, 27, 19,
  };

  const uint8_t JOINT_COUNT = sizeof(JOINT_PINS) / sizeof(JOINT_PINS[0]);

};


#endif  // CURRO_MOTOR_H_
