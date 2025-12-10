/**
  ******************************************************************************
  * @file    main.c
  * @author  Weili An, Niraj Menon
  * @date    Jan 15, 2024
  * @brief   ECE 362 Lab 2 Student template
  ******************************************************************************
*/


/**
******************************************************************************/

// Fill out your username, otherwise your completion code will have the 
// wrong username!
const char* username = "silva48";

/******************************************************************************
*/ 

#include "stm32f0xx.h"
#include <stdint.h>

void initc();
void initb();
void togglexn(GPIO_TypeDef *port, int n);
void init_exti();
void set_col(int col);
void SysTick_Handler();
void init_systick();
void adjust_priorities();

extern void autotest();
extern void internal_clock();
extern void nano_wait(int);

int main(void) {
    internal_clock();
    // Uncomment when most things are working
    autotest();
    
    initb();
    initc();
    init_exti();
    init_systick();
    adjust_priorities();

    // Slowly blinking
    for(;;) {
        togglexn(GPIOC, 9);
        nano_wait(500000000);
    }
}

/**
 * @brief Init GPIO port C
 *        PC0-PC3 as input pins with the pull down resistor enabled
 *        PC4-PC9 as output pins
 * 
 */
void initc() {
  RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
  GPIOC->MODER &= 0xfff00000;
  GPIOC->MODER |= 0x00055500;
  GPIOC->PUPDR |= 0x000000aa;
}

/**
 * @brief Init GPIO port B
 *        PB0, PB2, PB3, PB4 as input pins
 *          enable pull down resistor on PB2 and PB3
 *        PB8-PB11 as output pins
 * 
 */
void initb() {
  RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
  GPIOB->MODER &= 0xff00fc0c;
  GPIOB->MODER |= 0x00550000;
  GPIOB->PUPDR |= 0x000000a0;
  
}

/**
 * @brief Change the ODR value from 0 to 1 or 1 to 0 for a specified 
 *        pin of a port.
 * 
 * @param port : The passed in GPIO Port
 * @param n    : The pin number
 */
void togglexn(GPIO_TypeDef *port, int n) {
  port->ODR ^= 1 << (n); 
}

/**
 * @brief Follow the lab manual to initialize EXTI.  In a gist:
 *        (1-2) Enable the SYSCFG subsystem, and select Port B for
 *            pins 0, 2, 3, and 4.
 *        (3) Configure the EXTI_RTSR register so that an EXTI
 *            interrupt is generated on the rising edge of 
 *            each of the pins.
 *        (4) Configure the EXTI_IMR register so that the EXTI
 *            interrupts are unmasked for each of the pins.
 *        (5) Enable the three interupts for EXTI pins 0-1, 2-3 and
 *            4-15. Don't enable any other interrupts.
 */
void init_exti() {
  RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
  SYSCFG->EXTICR[0] |= 0x1101; 
  SYSCFG->EXTICR[1] |= 0x0001; 
  EXTI->RTSR |= 0x1d;
  EXTI->IMR |= 0x1d;
  NVIC->ISER[0] |= 0xe0;
}

//==========================================================
// Write the EXTI interrupt handler for pins 0 and 1 below.
// Copy the name from the startup file as explained in the 
// lab manual, create a label of that name below, and declare 
// it to be a function.
// It should acknowledge the pending bit for pin 0, and 
// it should call togglexn(GPIOB, 8).
void EXTI0_1_IRQHandler(){
  EXTI->PR = EXTI_PR_PR0;
  togglexn(GPIOB, 8);
} 


//==========================================================
// Write the EXTI interrupt handler for pins 2-3 below.
// It should acknowledge the pending bit for pin 2, and 
// it should call togglexn(GPIOB, 9).
void EXTI2_3_IRQHandler(){
  EXTI->PR = EXTI_PR_PR2;
  togglexn(GPIOB, 9);
}

//==========================================================
// Write the EXTI interrupt handler for pins 4-15 below.
// It should acknowledge the pending bit for pin 4, and 
// it should call togglexn(GPIOB, 10).
void EXTI4_15_IRQHandler(){
  EXTI->PR = EXTI_PR_PR4;
  togglexn(GPIOB, 10);
} 


/**
 * @brief Enable the SysTick interrupt to occur every 1/16 seconds.
 * 
 */
void init_systick() {
  SysTick->CTRL |=0x3;
  SysTick->CTRL &= ~0x4;
  SysTick->VAL = 0;
  SysTick->LOAD = 374999;
}

volatile int current_col = 1;

/**
 * @brief The ISR for the SysTick interrupt.
 * 
 */
void SysTick_Handler() {
  int temp = 0;
  temp = readpin(4-current_col);
  if(temp){
    togglexn(GPIOB, 7+current_col);
  }
  
  current_col++;
  if(current_col == 5){
    current_col = 1;
  }
  set_col(current_col);
  
}

/**
 * @brief For the keypad pins, 
 *        Set the specified column level to logic "high".
 *        Set the other three three columns to logic "low".
 * 
 * @param col 
 */
void set_col(int col) {
  GPIOC->ODR &= 0xffffff0f;
  GPIOC->ODR |= 1 << (8 - col);
}

/**
 * @brief Set the priority for EXTI pins 2-3 interrupt to 192.
 *        Set the priority for EXTI pins 4-15 interrupt to 128.
 *        Do not adjust the priority for any other interrupts.
 * 
 */
void adjust_priorities() {
  NVIC_SetPriority(EXTI2_3_IRQn,3);
  NVIC_SetPriority(EXTI4_15_IRQn,2);
  
}

int readpin(int pin_num) {
  int16_t num = 0;
  num = GPIOC->IDR;

  num = num << (0xf - pin_num);
  num = num >>(0xf);
  return ((int)num);
  
}

