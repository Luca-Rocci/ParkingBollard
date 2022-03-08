/*
  Hedge.h - Arduino library for the control of two servos 
  positioned one in front of the other in a synchronous way.
  Born by need to move a barrier up and down.

      +---+ |       | +---+       +-----------+
      |   |=|   +   |=|   |   =   |    the    | 
      |   | |       | |   |       |   hedge   |
      +---+           +---+      [ ]         [ ]
      Servo L       Servo R     Servo L   Servo R 
                          
  Copyright (c) 2020 Luca Rocci. All right reserved.
*/

#ifndef Hedge_h
#define Hedge_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#endif
#include <Servo.h>

class Hedge {

  public:
    Hedge();

    void attach(byte pinServoL, byte pinServoR);
    void setLowLevel(byte degre);
    void setHighLevel(byte degre);
    void startLowLevel();
    void startHighLevel();
    void runAllSteps();
    void runStep();
    void goUp();
    void goDown();
    bool isActive();

  private:
    Servo servoL;
    Servo servoR;
    byte lowLevel;
    byte highLevel; 
    byte shift;
    byte step;
    bool state;
    
};

#endif
