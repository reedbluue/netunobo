#include <Arduino.h>
#include <RobotController/RobotController.h>
#include <RobotMotor/RobotMotor.h>

#define LED 2
#define LEFT_SPIKE 22
#define RIGHT_SPIKE 23

RobotController psController1;
RobotMotor motorDD, motorDE, motorTD, motorTE;

long actNormalSpikeTime = millis();
long actRightSpikeTime = millis();
long actLeftSpikeTime = millis();

bool normalSpikeState = false;
bool rightSpikeState = false;
bool leftSpikeState = false;

void setup()
{
  pinMode(LED, OUTPUT);
  pinMode(LEFT_SPIKE, OUTPUT);
  pinMode(RIGHT_SPIKE, OUTPUT);

  Serial.begin(115200);
  Serial.println("Robot Started!");

  // Initialize the robot controller
  psController1 = RobotController("1a:2b:3c:01:01:01");

  // Initialize the robot motors
  motorDD = RobotMotor(33, 25, 32);
  motorDE = RobotMotor(5, 18, 19);
  motorTD = RobotMotor(27, 26, 14);
  motorTE = RobotMotor(16, 17, 4);
}

void loop()
{
  // Update the robot controller state
  psController1.updateController();

  if (millis() - actNormalSpikeTime > 100)
  {
    actNormalSpikeTime = millis();
    normalSpikeState = !normalSpikeState;
  }

  if (millis() - actRightSpikeTime < 100)
  {
    rightSpikeState = true;
  }
  else if (millis() - actRightSpikeTime >= 300)
  {
    actRightSpikeTime = millis();
  }
  else
  {
    rightSpikeState = false;
  }

  if (millis() - actLeftSpikeTime < 200)
  {
    leftSpikeState = false;
  }
  else if (millis() - actLeftSpikeTime >= 300)
  {
    actLeftSpikeTime = millis();
  }
  else
  {
    leftSpikeState = true;
  }

  if (psController1.R1 && psController1.L1)
  {
    // Activate both spikes
    digitalWrite(LEFT_SPIKE, leftSpikeState);
    digitalWrite(RIGHT_SPIKE, rightSpikeState);
  }
  else if (psController1.R1)
  {
    // Activate right spike
    digitalWrite(LEFT_SPIKE, false);
    digitalWrite(RIGHT_SPIKE, normalSpikeState);
  }
  else if (psController1.L1)
  {
    // Activate left spike
    digitalWrite(LEFT_SPIKE, normalSpikeState);
    digitalWrite(RIGHT_SPIKE, false);
  }
  else
  {
    // Deactivate both spikes
    digitalWrite(LEFT_SPIKE, false);
    digitalWrite(RIGHT_SPIKE, false);
  }

  int acceleration = psController1.R2Value;
  int brake = psController1.L2Value;
  int speed = acceleration - brake;
  int direction = psController1.LXValue;
  int rotation = psController1.RXValue;
  int rawSpeed = abs(speed);

  if (rotation > 0)
  {
    // Rotate right
    motorDD.run(100, true);
    motorDE.run(100);
    motorTD.run(100, true);
    motorTE.run(100);
  }
  else if (rotation < 0)
  {
    // Rotate left
    motorDD.run(100);
    motorDE.run(100, true);
    motorTD.run(100);
    motorTE.run(100, true);
  }
  else if (psController1.R2 && psController1.R2 && speed == 0)
  {
    // Handle different movement directions

    bool north = false, northEast = false, east = false, southEast = false, south = false, southWest = false, west = false, northWest = false;

    if (psController1.LYValue >= 95)
      north = true;
    else if (psController1.LYValue <= -95)
      south = true;
    else if (psController1.LXValue >= 95)
      east = true;
    else if (psController1.LXValue <= -95)
      west = true;
    else if (psController1.LXValue > 0 && psController1.LXValue > 40 && psController1.LYValue > 0 && psController1.LYValue > 40)
      northEast = true;
    else if (psController1.LXValue > 0 && psController1.LXValue > 40 && psController1.LYValue < 0 && psController1.LYValue < -40)
      southEast = true;
    else if (psController1.LXValue < 0 && psController1.LXValue < -40 && psController1.LYValue < 0 && psController1.LYValue < -40)
      southWest = true;
    else if (psController1.LXValue < 0 && psController1.LXValue < -40 && psController1.LYValue > 0 && psController1.LYValue > 40)
      northWest = true;

    if (north)
    {
      // Move forward
      motorDD.run(100);
      motorDE.run(100);
      motorTD.run(100);
      motorTE.run(100);
    }
    else if (south)
    {
      // Move backward
      motorDD.run(100, true);
      motorDE.run(100, true);
      motorTD.run(100, true);
      motorTE.run(100, true);
    }
    else if (west)
    {
      // Move left
      motorDD.run(100);
      motorDE.run(100, true);
      motorTD.run(100, true);
      motorTE.run(100);
    }
    else if (east)
    {
      // Move right
      motorDD.run(100, true);
      motorDE.run(100);
      motorTD.run(100);
      motorTE.run(100, true);
    }
    else if (northEast)
    {
      // Move northeast
      motorDD.stop();
      motorDE.run(100);
      motorTD.run(100);
      motorTE.stop();
    }
    else if (southEast)
    {
      // Move southeast
      motorDD.run(100, true);
      motorDE.stop();
      motorTD.stop();
      motorTE.run(100, true);
    }
    else if (southWest)
    {
      // Move southwest
      motorDD.stop();
      motorDE.run(100, true);
      motorTD.run(100, true);
      motorTE.stop();
    }
    else if (northWest)
    {
      // Move northwest
      motorDD.run(100);
      motorDE.stop();
      motorTD.stop();
      motorTE.run(100);
    }
    else
    {
      // Stop all motors
      motorDD.stop();
      motorDE.stop();
      motorTD.stop();
      motorTE.stop();
    }
  }
  else
  {
    if (direction > 0 && speed == 0)
    {
      // Turn right
      motorDD.run(100, true);
      motorDE.run(100);
      motorTD.run(100);
      motorTE.run(100, true);
    }
    else if (direction < 0 && speed == 0)
    {
      // Turn left
      motorDD.run(100);
      motorDE.run(100, true);
      motorTD.run(100, true);
      motorTE.run(100);
    }
    else if (speed < 0)
    {
      // Move backward with variable speed

      if (psController1.LXValue < 0)
      {
        motorDD.run(rawSpeed, true);
        motorDE.run(rawSpeed - abs(psController1.LXValue), true);
        motorTD.run(rawSpeed, true);
        motorTE.run(rawSpeed - abs(psController1.LXValue), true);
      }
      else if (psController1.LXValue > 0)
      {
        motorDD.run(rawSpeed - abs(psController1.LXValue), true);
        motorDE.run(rawSpeed, true);
        motorTD.run(rawSpeed - abs(psController1.LXValue), true);
        motorTE.run(rawSpeed, true);
      }
      else
      {
        motorDD.run(rawSpeed, true);
        motorDE.run(rawSpeed, true);
        motorTD.run(rawSpeed, true);
        motorTE.run(rawSpeed, true);
      }
    }
    else if (speed > 0)
    {
      // Move forward with variable speed

      if (psController1.LXValue > 0)
      {
        motorDD.run(rawSpeed - abs(psController1.LXValue));
        motorDE.run(rawSpeed);
        motorTD.run(rawSpeed - abs(psController1.LXValue));
        motorTE.run(rawSpeed);
      }
      else if (psController1.LXValue < 0)
      {
        motorDD.run(rawSpeed);
        motorDE.run(rawSpeed - abs(psController1.LXValue));
        motorTD.run(rawSpeed);
        motorTE.run(rawSpeed - abs(psController1.LXValue));
      }
      else
      {
        motorDD.run(rawSpeed);
        motorDE.run(rawSpeed);
        motorTD.run(rawSpeed);
        motorTE.run(rawSpeed);
      }
    }
    else
    {
      // Stop all motors
      motorDD.stop();
      motorDE.stop();
      motorTD.stop();
      motorTE.stop();
    }
  }
}