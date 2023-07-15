#include <PS4Controller.h>
#include <Arduino.h>

#define OFFSET 20

class RobotController{
  
  public:
    PS4Controller source = PS4;
    bool triggerCross = false;
    bool triggerCircle = false;
    bool triggerTriangle = false;
    bool triggerSquare = false;

    bool cross = false;
    bool R1 = false;
    bool L1 = false;

    bool L2 = false;
    int L2Value = 0;
    bool R2 = false;
    int R2Value = 0;
    int LXValue = 0;
    int LYValue = 0;
    int RXValue = 0;
    int RYValue = 0;
  

  private:
    bool connected = false;
    bool antCross = false;
    bool antCircle = false;
    bool antTriangle = false;
    bool antSquare = false;

  public:
    RobotController() {
    }
    RobotController(char *mac) {
      PS4.begin(mac); //"40:99:22:b3:2c:c6"
      Serial.println("Controller service has started!\nWaiting connection...");
    }

    /**
     * Updates the controller by checking if it is connected and updating LED if necessary.
     */
    void updateController() {
      this->checkIfConnected();

      if(millis() % 2000 == 0 && connected) {
        this->updateLed();
      }

      this->triggerCross = detectTrigger(PS4.Cross(), &this->antCross);
      this->triggerCircle = detectTrigger(PS4.Circle(), &this->antCircle);
      this->triggerTriangle = detectTrigger(PS4.Triangle(), &this->antTriangle);
      this->triggerSquare = detectTrigger(PS4.Square(), &this->antSquare);

      this->cross = PS4.Cross();

      this->L2 = PS4.R2();
      this->L2Value = map(PS4.L2Value(), 0, 255, 0, 100);
      this->R2 = PS4.R2();
      this->R2Value = map(PS4.R2Value(), 0, 255, 0, 100);

      this->LXValue = map(PS4.LStickX(), -127, 127, -100, 100);
      this->LXValue = this->LXValue > -OFFSET && this->LXValue < OFFSET ? 0 : this->LXValue;

      this->LYValue = map(PS4.LStickY(), -127, 127, -100, 100);
      this->LYValue = this->LYValue > -OFFSET && this->LYValue < OFFSET ? 0 : this->LYValue;
      
      this->RXValue = map(PS4.RStickX(), -127, 127, -100, 100);
      this->RXValue = this->RXValue > -OFFSET && this->RXValue < OFFSET ? 0 : this->RXValue;

      this->RYValue = map(PS4.RStickY(), -127, 127, -100, 100);
      this->RYValue = this->RYValue > -OFFSET && this->RYValue < OFFSET ? 0 : this->RYValue;

      this->R1 = PS4.R1();
      this->L1 = PS4.L1();
    }

  private:
    void checkIfConnected() {
      if (PS4.isConnected() && !this->connected)
      {
        this->connected = true;
        Serial.println("The controller has connected!");
      }

      if (!PS4.isConnected() && this->connected)
      {
        this->connected = false;
        this->reset();
        Serial.println("The controller has disconnected!");
      }
    }

    void updateLed() {
      int battery = PS4.Battery();
      
      int redLevel = map(battery, 0, 100, 0, 255);
      int greenLevel = map(battery, 0, 100, 255, 0);

      PS4.setLed(redLevel, greenLevel, 0);
      PS4.sendToController();
    }

    bool detectTrigger(bool button, bool * antVal) {
      bool trigger = button && !*antVal ? true : false;
      *antVal = button;
      return trigger;
    }

    void reset() {
      this->triggerCross = false;
      this->triggerCircle = false;
      this->triggerTriangle = false;
      this->triggerSquare = false;
      this->L2 = false;
      this->L2Value = 0;
      this->R2 = false;
      this->R2Value = 0;
      this->LXValue = 0;
      this->LYValue = 0;
      this->RXValue = 0;
      this->RYValue = 0;
      this->cross = false;
      this->R1 = false;
      this->L1 = false;
    }
};