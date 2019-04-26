#include "motor_controller.h"

void MotorController::Rotate() {
  GPIO_PORTD_DATA_R &= ~0x0F;
  GPIO_PORTD_DATA_R |= 0x01; // PD0 controls the direction of motor 
  StartTimer(0x989680); // 10 Million
  SpeedAdjustment(3);
  while ((TIMER0_RIS_R & 0x00000001) == 0);
  //SpeedAdjustment(1);
  EndTimer();
}

void MotorController::Configure() {
  ConfigurePWM();
  ConfigureGPIO();
  ConfigureTimer();
}

void MotorController::ConfigurePWM() {
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
  SysCtlPWMClockSet(SYSCTL_PWMDIV_1);
  (*(volatile unsigned long *)(GPIO_PORTF_BASE + GPIO_O_LOCK)) = GPIO_LOCK_KEY;
  (*(volatile unsigned long *)(GPIO_PORTF_BASE + GPIO_O_CR)) |= 0x01;
  GPIOPinConfigure(GPIO_PF2_M1PWM6);
  GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_2);
  PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
  PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, 400);
  SpeedAdjustment(0);
}

void MotorController::ConfigureGPIO() {
  // Use GPIO Port D for motor direction.
  SYSCTL_RCGCGPIO_R |= 0x08;
  GPIO_PORTD_DIR_R = 0x0F;
  GPIO_PORTD_DEN_R = 0x0F;
  GPIO_PORTD_DATA_R &= ~0x0F;
}

void MotorController::ConfigureTimer() {
  SYSCTL_RCGCTIMER_R |= 0x01;
  TIMER0_CTL_R &= ~0x00000001;
  TIMER0_CFG_R = 0x00000000;
  TIMER0_TAMR_R = 0x00000002;
  TIMER0_ICR_R |= 0x00000001;
}

void MotorController::SpeedAdjustment(int level) {
  switch(level) {
    case 0:
      PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, 0);
      break;
    case 1:
      PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, 100);
      break;
    case 2:
      PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, 150);
      break;
    case 3:
      PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, 250);
      break;
    case 4:
      PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, 395);
      break;
    default:
      PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, 50);
      break;
  }
  PWMGenEnable(PWM1_BASE, PWM_GEN_3);
  PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT, true);
}

void MotorController::StartTimer(int load_value) {
  // Load count-down value
  TIMER0_TAILR_R = load_value; // 0x00F42400
  // Enable timer
  TIMER0_CTL_R |= 0x00000001;
}

void MotorController::EndTimer() {
  // Disable timer
  TIMER0_CTL_R &= ~0x00000001;
  // Clear flag
  TIMER0_ICR_R |= 0x00000001;
}