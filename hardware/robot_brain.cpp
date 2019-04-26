#include <iostream>
#include "robot_brain.h"

void RobotBrain::Work() {
  // bool autoMode = true;
  // Configure controllers
  //bool forward = true;
  motorController->Configure();
  distanceController->Configure();
//  uartController->Configure();
//  uartController->Write("Hello, world. This is Group 12!\r\n");
//  uartController->Write("Please press k for automatic or l for manual mode.\r\n");
//  // Get driving mode
//  char mode = uartController->Read();
//  if (mode == 'l') {
//    autoMode = false;
//    uartController->Write("Manual controls: a -> left, s -> back, d -> right\r\n");
//  }
  // Start driving
  while (1) {
    bool forward = distanceController->CheckObstacleForward();
   // bool forward = true;
    if (!forward) {
      // Whether it's in automatic or manual mode, move forward if there isn't
      // any obstacle.
      motorController->Rotate();
      // uartController->Write("Forward!\r\n");
    } 
  }
}