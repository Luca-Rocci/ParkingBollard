/*
  ParkingBollard.ino - Arduino project for the  
  remote control of a parking bollard. 
  Consisting of two servos and a 433 MHz receiver.

    ) ) ) Y ( ( (            o=========o
          |                 ||   the   || 
          |                 ||  hedge  ||
        [433]-----[I/O]----[L]---------[R]
       Receiver  Arduino  Servo       Servo 
                          
  Copyright (c) 2020 Luca Rocci. All right reserved.
*/

#include "Hedge.h"
#include <RCSwitch.h>

RCSwitch receiver;
Hedge hedge;
int ledCounter = 0;

#define BUTTON_A 8648017
#define BUTTON_B 8648018

#define DEBUG 0 //!< Debug mode

#define LED 0 //!< Enable led built-in

// The setup function runs once when you press reset or power the board
void setup() {
  #if LED == 1
  // Initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  // Initialize digital pin RX LED as an output.
  pinMode(0, OUTPUT);
  // Initialize digital pin TX LED as an output.
  pinMode(1, OUTPUT);
  #endif
  
  hedge.attach(10,11);
  hedge.setHighLevel(90);
  hedge.setLowLevel(0);
  
  // Set the initial position of the barrier in the down
  hedge.startLowLevel();
  // Receiver on interrupt 0 => that is pin 2
  receiver.enableReceive(0);

  #if DEBUG == 1
  Serial.begin(9600);
  Serial.println("End Setup");
  #endif
}

// The loop function runs over and over again forever
void loop() {
  if (receiver.available()) {
    unsigned long res = receiver.getReceivedValue(); 
    switch (res) {
      case BUTTON_A:        // se arriva il a
        hedge.goUp();
        #if LED == 1
        digitalWrite(0, HIGH);   // Turn the LED on (HIGH is the voltage level)
        digitalWrite(1, LOW);    // Turn the LED off by making the voltage LOW
        #endif
        #if DEBUG == 1
        Serial.println("Button A");
        #endif
        break;                           
      case BUTTON_B:     // se arriva il b
        #if LED == 1
      digitalWrite(0, LOW);    // Turn the LED off by making the voltage LOW
      digitalWrite(1, HIGH);   // Turn the LED on (HIGH is the voltage level)
        #endif
        hedge.goDown();
        #if DEBUG == 1
        Serial.println("Button B");
        #endif                
        break; 
    }   
    receiver.resetAvailable();
  }
  hedge.runStep();
  #if DEBUG == 1
 // Serial.print("runStep");
  #endif
  if (hedge.isActive()) {
    #if LED == 1
      digitalWrite(LED_BUILTIN, HIGH);   // Turn the LED on (HIGH is the voltage level)
      #endif
      #if DEBUG == 1
        Serial.println("Led on - Servo on");
      #endif
  } else {
    if(ledCounter <= 20000) {
      #if LED == 1
      digitalWrite(LED_BUILTIN, LOW);    // Turn the LED off by making the voltage LOW
      #endif
      #if DEBUG == 1
        Serial.println("Led off");
      #endif
    } else {
      #if DEBUG == 1
        Serial.println("Led on");
      #endif
      #if LED == 1
      digitalWrite(LED_BUILTIN, HIGH);   // Turn the LED on (HIGH is the voltage level)
      #endif
    } 
    ledCounter++;
    if(ledCounter >= 40000) ledCounter = 0;
  }
}
