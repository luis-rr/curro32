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
    
    static constexpr float MIN_ANGLE = 0.0f;
    static constexpr float MAX_ANGLE = 290.f;
    static constexpr float MID_ANGLE = (MAX_ANGLE - MIN_ANGLE) * 0.5f;
    
    static const int PW_WIDTH_MIN = 500;
    static const int PW_WIDTH_MAX = 2500;
    static const int MAX_RES_BITS = 16;
    static const int DUTY_MAX = (1 << MAX_RES_BITS) - 1;

    const float US_TO_S = 1000 * 1000;

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
    Servo(int pin, int channel, bool flipped);
    ~Servo();

    void write(float degrees) {

      float target = degrees + MID_ANGLE;

      if (_flipped) {
        target = MAX_ANGLE - target;
      }

      _writeTarget(target);      
    }

  private:

    void _writeTarget(float degrees);

    const bool _flipped;
    const int _pin;
    const int _channel;
};

class Body {
public:

  /*

    PIN, PW channel:

    --------------------------
    |    4,  9, frkn     2, 10, brkn  |
    |    5,  5, frsh    15,  6, brsh  |
    |    18, 1, neck    13,  2, ----  |
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

  Body() {
    Serial.println("body built");
  }

  Servo neck{18, 0, false};
  
  Servo frsh{5, 1, false};
  Servo brsh{15, 2, false};
  Servo blsh{14, 3, true};
  Servo flsh{33, 4, true};

  Servo frkn{4, 5, true};
  Servo brkn{2, 6, true};
  Servo blkn{27, 7, false};
  Servo flkn{19, 8, false};

  const byte JOINT_COUNT = 9;
};


class MotorHandler {
  float angle = 150;
  int dir = 1;

  Body* body;

public:
  bool setup();
  void loop();
};


#endif  // CURRO_MOTOR_H_
