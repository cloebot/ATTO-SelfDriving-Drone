//
// Created by Mert Akozcan on 11/26/17.
//

#include "distance_controller.h"
#include <stdio.h>

bool DistanceController::CheckObstacleForward() {
  // PE3 is connected to forward facing sensor.
  ADC0_PSSI_R |= 0x8;
  // Wait for conversion to be completed
  while ((ADC0_RIS_R & 0x8) == 0);
  int result = ADC0_SSFIFO3_R;
  printf("result: %d\n", result);
  // Clear SS3 flag
  ADC0_ISC_R |= (1<<3);
  // Check whether there is an obstacle in forward direction.
  return !IsObstacle(result);  
}

bool DistanceController::IsObstacle(int result) {
  float volts = ((float)result * 3.3f) / 4095.0f; // Input 3.3V
  float distance = 27.86f * (1.0f / (pow(volts, 1.15f)));
  return (distance > 17.0f);
}

void DistanceController::Configure() {
  ConfigureADC();
}

void DistanceController::ConfigureADC() {
  // Enable clock for GPIO Port E.
  SYSCTL_RCGCGPIO_R |= 0x10;
  // Enable clock for ADC0
  SYSCTL_RCGCADC_R |= 0x1;
  while ((SYSCTL_RCGCADC_R & 0x0000001) == 0) {};
  
  // Enable alternate function
  GPIO_PORTE_AFSEL_R |= 0xE;
  // Disable digital function
  GPIO_PORTE_DEN_R &= ~0xE;
  // Enable analog function
  GPIO_PORTE_AMSEL_R |= 0xE;
  
  // Disable SS3 before configuration
  ADC0_ACTSS_R &= ~0xE;
  // Software trigger conversion
  ADC0_EMUX_R &= ~0xFFF0;
  // Set input channels
  ADC0_SSMUX3_R |= 0x0;
  
  // Take one sample at a time, set flag at first sample
  // Read data from input channels
  ADC0_SSCTL3_R |= 0x6;
  ADC0_SSCTL3_R &= ~(1<<3); 
  
  // Enable SS3
  ADC0_ACTSS_R |= 0xE;
  
  // Clear flags
  ADC0_ISC_R |= (1<<3);
}