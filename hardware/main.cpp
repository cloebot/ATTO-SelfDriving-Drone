#include "robot_brain.h"
#include "stdio.h"

int main()
{
  printf("hi\n");
  RobotBrain *robotBrain = new RobotBrain();
  robotBrain->Work();
  delete robotBrain;
  return 0;
}
