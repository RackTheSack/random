/**
  ******************************************************************************
  * @file    main.c
  * @author  Weili An, Niraj Menon
  * @date    Jan 5 2024
  * @brief   ECE 362 Lab 1 template
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

void initb();
void initc();
void setn(int32_t pin_num, int32_t val);
int32_t readpin(int32_t pin_num);
void buttons(void);
void keypad(void);
void autotest(void);
extern void internal_clock(void);
extern void nano_wait(unsigned int n);

int main(void) {
    internal_clock(); // do not comment!
    // Comment until most things have been implemented
    autotest();
    initb();
    initc();

    // uncomment one of the loops, below, when ready
    // while(1) {
    //   buttons();
    // }

    // while(1) {
    //   keypad();
    // }

    for(;;);
    
    return 0;
}

/**
 * @brief Init GPIO port B
 *        Pin 0: input
 *        Pin 4: input
 *        Pin 8-11: output
 *
 */
void initb() {
  RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
  GPIOB->MODER &= 0xff00fcfc;
  GPIOB->MODER |= 0x00550000;
}

/**
 * @brief Init GPIO port C
 *        Pin 0-3: inputs with internal pull down resistors
 *        Pin 4-7: outputs
 *
 */
void initc() {
  RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
  GPIOC->MODER &= 0xffff0000;
  GPIOC->MODER |= 0x00005500;
  GPIOC->PUPDR |= 0x000000aa;
  
}

/**
 * @brief Set GPIO port B pin to some value
 *
 * @param pin_num: Pin number in GPIO B
 * @param val    : Pin value, if 0 then the
 *                 pin is set low, else set high
 */
void setn(int32_t pin_num, int32_t val) {
  int32_t num = 0x1;
  num = num << pin_num;
  if(val == 0){
    GPIOB->ODR &= ~num;
  }
  else {
    GPIOB->ODR |= num;
  }
}

/**
 * @brief Read GPIO port B pin values
 *
 * @param pin_num   : Pin number in GPIO B to be read
 * @return int32_t  : 1: the pin is high; 0: the pin is low
 */
int32_t readpin(int32_t pin_num) {
  int32_t num = 0;
  num = GPIOB->IDR;

  num = num << (0xf - pin_num);
  num = num >>(0xf);
  return (num);
  
}

/**
 * @brief Control LEDs with buttons
 *        Use PB0 value for PB8
 *        Use PB4 value for PB9
 *
 */
void buttons(void) {
  int32_t temp;
  temp = readpin(0);
  setn(8,temp);
  temp = readpin(4);
  setn(9,temp);
  
}

/**
 * @brief Control LEDs with keypad
 * 
 */
void keypad(void) {
  int row;
  for(int32_t i = 0;i < 4; i++){
    GPIOC->ODR |= 1 << (7 - i); 
    nano_wait(1000000);
    row = GPIOC->IDR & 0xF;
    setn((8+i), (row&(1<<(3-i))));

  }
  //loop i = 1, 2, 3, 4 {
   //     set ith column to be 1 using GPIOC->ODR
   //     call nano_wait(1000000)
   //     read the row inputs PC0-PC3 using GPIOC->IDR & 0xF
   //     check the ith row value and set this to ith LED output pin using `setn`
   // }
  
}
