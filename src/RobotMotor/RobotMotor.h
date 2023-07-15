#include <Arduino.h>

class RobotMotor {

  public:
    inline static int globalAnalogChannel = 0;
    inline static int MIN_SPEED = 0;
    inline static int MAX_SPEED = 255;

  private:
    int in1, in2, inSpeed;
    int analogChannel;
    bool start = false, reverse = false;

  public:
    RobotMotor(){}

    RobotMotor(int in1, int in2, int inSpeed) {
      if(RobotMotor::globalAnalogChannel >= 15) {
        throw std::invalid_argument("Max channel reached!");
      }
    
      this->in1 = in1;
      this->in2 = in2;
      this->inSpeed = inSpeed;
      this->analogChannel = RobotMotor::globalAnalogChannel;

      pinMode(in1, OUTPUT);
      pinMode(in2, OUTPUT);

      ledcSetup(this->analogChannel, 1000, 8);
      ledcAttachPin(inSpeed, this->analogChannel);
      RobotMotor::globalAnalogChannel++;
    }

  public:
    /**
     * Runs the motor at the specified speed.
     *
     * @param speed the speed at which the motor should run
     * @param reverse (optional) whether the motor should run in reverse (default: false)
     */
    void run(int speed, bool reverse = false) {
      if(reverse) {
        digitalWrite(this->in1, true);
        digitalWrite(this->in2, false);
      } else {
        digitalWrite(this->in1, false);
        digitalWrite(this->in2, true);
      }
      ledcWrite(this->analogChannel, this->getSpeed(speed));
    }

    /**
     * Stops the motor by setting both input pins to false and writing 0 to the analog channel.
     */
    void stop() {
      digitalWrite(in1, false);
      digitalWrite(in2, false);
      ledcWrite(this->analogChannel, 0);
    }

  private:
    int getSpeed(int speed) {
      return map(speed, 0, 100, RobotMotor::MIN_SPEED, RobotMotor::MAX_SPEED);
    }
};