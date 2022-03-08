/*
  Hedge.cpp - Arduino library for the control of two servos 
  positioned one in front of the other in a synchronous way.
  Born by need to move a barrier up and down.

      +---+ |       | +---+       +-----------+
      |   |=|   +   |=|   |   =   |    the    | 
      |   | |       | |   |       |   hedge   |
      +---+           +---+      [ ]         [ ]
      Servo L       Servo R     Servo L   Servo R 
                          
  Copyright (c) 2020 Luca Rocci. All right reserved.
*/

#include "Hedge.h"

Hedge::Hedge() {
  // Set the value to 0 so the library is not active. 
  // It is calculated in the startLowLevel or startHighLevel methods
  this->shift = 0; 
  this->lowLevel = 0;   // default value
  this->highLevel = 90; // default value
}

/**
 * Attaches the servo left and right
 *
 * @param pinServoL      the number of the pin that the servo is attached 
 * @param pinServoR      the number of the pin that the servo is attached 
 */
void Hedge::attach(byte pinServoL, byte pinServoR) {
  // Attaches the servo left on pinServoL to the servoL object
  this->servoL.attach(pinServoL); 
  // Attaches the servo right on pin pinServoR to the servoR object
  this->servoR.attach(pinServoR); 
}

/**
 * Set the servo degree in down position
 * e.g.: 0° default value
 *
 * @param degree      Number (0..180)
 */
void Hedge::setLowLevel(byte degree) {
  this->lowLevel = degree; 
}

/**
 * Set the servo degree in raiser position
 * e.g.: 90° default value
 *
 * @param degree      Number (0..180)
 */
void Hedge::setHighLevel(byte degree) {
  this->highLevel = degree; 
}

/**
 * Set the initial position of the barrier 
 * in the down
 *                     +-------+-----+
 *                     | level | e.g.|
 * +-------------------+-------+-----+
 * | servo right       | low   | 0°  |
 * | servo left        | high  | 90° |
 * +-------------------+-------+-----+
 */
void Hedge::startLowLevel() {
  this->servoL.write(this->lowLevel);
  this->servoR.write(this->highLevel);
  this->shift = this->highLevel - this->lowLevel;
  this->step = this->shift;
  this->state = false;
}

/**
 * Set the initial position of the barrier 
 * in the up
 *                     +-------+-----+
 *                     | level | e.g.|
 * +-------------------+-------+-----+
 * | servo right       | high  | 90° |
 * | servo left        | low   |  0° |
 * +-------------------+-------+-----+
 */
void Hedge::startHighLevel() {
  this->servoL.write(this->highLevel);
  this->servoR.write(this->lowLevel);
  this->shift = this->highLevel - this->lowLevel;
  this->step = this->shift;
  this->state = true;
}
    
/**
 * Set the barrier state to move up
 */
void Hedge::goUp() {
  if(!this->state) {
    this->step = this->shift - this->step;
    this->state = true;
  }
}

/**
 * Set the barrier state to move down
 */
void Hedge::goDown() {
  if(this->state) {
    this->step = this->shift - this->step;
    this->state = false;
  }
}

/**
 * Moves the barrier up to the set state
 *                     +-------+------+
 *  e.g.               | start | end  |
 * +-------------------+-------+------+
 * | servo right       |  high | low  |
 * | servo left        |  low  | high |
 * +-------------------+-------+------+
 */
void Hedge::runAllSteps() {
  // Goes from the current step to the shift value
  for (int i = this->step; i <= this->shift; i++) { 
    this->runStep(); // Moves the barrier
  }
}

/**
 * Moves the barrier one step according to the state
 *                     +-------+----------+
 *  e.g.               | start | end      |
 * +-------------------+-------+----------+
 * | servo right       |  high | high - 1 |
 * | servo left        |  low  | low + 1  |
 * +-------------------+-------+----------+
 */
void Hedge::runStep() {
  // Check if necessary a shift
  if(this->shift == this->step) return;
  // Check the status and tell the servos to go to the next position
  if(this->state) {
    this->servoL.write(this->lowLevel + this->step);
    this->servoR.write(this->highLevel - this->step);
  } else {
    this->servoL.write(this->highLevel - this->step);
    this->servoR.write(this->lowLevel + this->step);
  }
  delay(15);    // Waits 15ms for the servo to reach the position
  this->step++; // Increase the step
}

/**
 * Check if the barrier is moving
 * 
 * @return true if necessary a shift
 */
bool Hedge::isActive() {
  // Check if necessary a shift
  return (this->shift != this->step);
}
