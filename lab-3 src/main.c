/**
  ******************************************************************************
  * @file    main.c
  * @author  Weili An, Niraj Menon
  * @date    Jan 19, 2024
  * @brief   ECE 362 Lab 3 Student template
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
#include <stdio.h>
#include <stdint.h>

// Global data structure
char disp[9]         = "Hello...";
uint8_t col          = 0;
uint8_t mode         = 'A';
uint8_t thrust       = 0;
int16_t fuel         = 800;
int16_t alt          = 4500;
int16_t velo         = 0;

// Keymap is in `font.S` to match up what autotester expected
extern char keymap;
extern char disp[9];
extern uint8_t col;
extern uint8_t mode;
extern uint8_t thrust;
extern int16_t fuel;
extern int16_t alt;
extern int16_t velo;

// Make it easier to access keymap
char* keymap_arr = &keymap;

// Font array in assembly file
// as I am too lazy to convert it into C array
extern uint8_t font[];

// The functions we should implement
void enable_ports();
void show_char(int n, char c);
void drive_column(int c);
int read_rows();
char rows_to_key(int rows);
void handle_key(char key);
void setup_tim7();
void write_display();
void update_variables();
void setup_tim14();

// Auotest functions
void internal_clock();
extern void check_wiring();
extern void autotest();
extern void fill_alpha();

int main(void) {
    internal_clock();

    // Uncomment when you are ready to test wiring.
    //check_wiring();
    
    // Uncomment when you are ready to test everything.
    autotest();
    
    enable_ports();
    // Comment out once you are checked off for fill_alpha
    fill_alpha();

    setup_tim7();
    setup_tim14();

    for(;;) {
        // enter low power sleep mode 
        // and Wait For Interrupt (WFI)
        asm("wfi");
    }
}

/**
 * @brief Enable the ports and configure pins as described
 *        in lab handout
 * 
 */
void enable_ports() {
  RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
  RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
  GPIOB->MODER &= 0xffc00000;
  GPIOB->MODER |= 0x00155555;
  GPIOC->MODER &= 0xffff0000;
  GPIOC->MODER |= 0x00005500;
  GPIOC->PUPDR |= 0x000000aa;
    
}

/**
 * @brief Show a character `c` on column `n`
 *        of the segment LED display
 * 
 * @param n 
 * @param c 
 */
void show_char(int n, char c) {
  if(!(0 <= n) || !(n <= 7)){
    return;
  }
  GPIOB->ODR = n << 8 | font[c]; 

  return;
    
}

/**
 * @brief Drive the column pins of the keypad
 *        First clear the keypad column output
 *        Then drive the column represented by `c`
 * 
 * @param c 
 */
void drive_column(int c) {
  c &= 0x00000003;
  GPIOC->BRR |= 0xf0;
  GPIOC->BSRR |= 1 << c+4;
}

/**
 * @brief Read the rows value of the keypad
 * 
 * @return int 
 */
int read_rows() {
  int16_t temp = 0;
  temp = GPIOC->IDR;
  temp &= 0xf;
  return(temp);
}

/**
 * @brief Convert the pressed key to character
 *        Use the rows value and the current `col`
 *        being scanning to compute an offset into
 *        the character map array
 * 
 * @param rows 
 * @return char 
 */
char rows_to_key(int rows) {
  char c = 0;
  int16_t val = 0;
  if(rows & 0x1){
    val = 0;
  }
  else if (rows & 0x2){
    val = 1;
  }
  else if (rows & 0x4){
    val = 2;
  }
  else if(rows & 0x8){
    val = 3;
  }

  c = col * 4 + val;

  return keymap_arr[c];
    
}

/**
 * @brief Handle key pressed in the game
 * 
 * @param key 
 */
void handle_key(char key) {
  if(key == 'A' || key == 'B' || key == 'D'){
    mode = key;
  }
  else if(key == '1' || key == '2' || key == '3' || key == '4' || key == '5' || key == '6' || key == '7' || key == '8' || key == '9' || key == '0'){
    thrust = key - '0';
  }
    
}

//-------------------------------
// Timer 7 ISR goes here
//-------------------------------
// TODO
void TIM7_IRQHandler(){
  TIM7->SR &= ~TIM_SR_UIF;
  int rows = 0;
  char c = 0;
  char key;
  rows = read_rows();
  if(rows != 0){
    key = rows_to_key(rows);
    handle_key(key);
  }
  c = disp[col];
  show_char(col,c);
  col = col +1;
  if (col > 7){
    col = 0;
  }
  drive_column(col);
}

/**
 * @brief Setup timer 7 as described in lab handout
 * 
 */
void setup_tim7() {
  RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;
  TIM7->PSC = 48-1;
  TIM7->ARR = 1000-1;
  TIM7->DIER |= TIM_DIER_UIE;
  NVIC->ISER[0] |= 1 << TIM7_IRQn;
  TIM7->CR1 |= TIM_CR1_CEN;

}


/**
 * @brief Write the display based on game's mode
 * 
 */
void write_display() {
  if(mode == 'C'){
    snprintf(disp, 9, "Crashed");
  }
  else if(mode == 'L'){
    snprintf(disp, 9, "Landed ");
  }
  else if(mode == 'A'){
    snprintf(disp, 9, "ALt%5d", alt);
  }
  else if(mode == 'B'){
    snprintf(disp, 9, "FUEL %3d", fuel);
  }
  else if(mode == 'D'){
    snprintf(disp, 9, "Spd %4d", velo);
  }

    
}

/**
 * @brief Game logic
 * 
 */
void update_variables() {
  fuel = fuel - thrust;
  if(fuel <= 0){
    thrust =0;
    fuel = 0;
  }

  alt += velo;
  if(alt <= 0){
    if((velo*-1) < 10){
      mode = 'L';
    }
    else{
      mode = 'C';
    }
    return;
  }
  velo += thrust -5;

}


//-------------------------------
// Timer 14 ISR goes here
//-------------------------------
// TODO
void TIM14_IRQHandler(){
  TIM14->SR &= ~TIM_SR_UIF;
  update_variables();
  write_display();
}

/**
 * @brief Setup timer 14 as described in lab
 *        handout
 * 
 */
void setup_tim14() {
  RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;
  TIM14->PSC = 24000-1;
  TIM14->ARR = 1000-1;
  TIM14->DIER |= TIM_DIER_UIE;
  NVIC->ISER[0] |= 1 << TIM14_IRQn;
  TIM14->CR1 |= TIM_CR1_CEN;
}


