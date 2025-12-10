/**
  ******************************************************************************
  * @file    main.c
  * @author  Weili An, Niraj Menon
  * @date    Feb 7, 2024
  * @brief   ECE 362 Lab 7 student template
  ******************************************************************************
*/

/*******************************************************************************/

// Fill out your username!  Even though we're not using an autotest, 
// it should be a habit to fill out your username in this field now.
const char* username = "silva48";

/*******************************************************************************/ 

#include "stm32f0xx.h"
#include <stdint.h>

void internal_clock();

// Uncomment only one of the following to test each step
//#define STEP1
//#define STEP2
//#define STEP3
#define STEP4

void init_usart5() {
    // TODO
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    RCC->AHBENR |= RCC_AHBENR_GPIODEN;
    GPIOC->MODER &= ~(GPIO_MODER_MODER12);
    GPIOC->MODER |=  GPIO_MODER_MODER12_1;
    GPIOC->AFR[1] &= 0xfff0ffff;
    GPIOC->AFR[1] |= 0x00020000;
    GPIOD->MODER &= ~(GPIO_MODER_MODER2);
    GPIOD->MODER |=  GPIO_MODER_MODER2_1;
    GPIOD->AFR[0] &= 0xfffff0ff;
    GPIOD->AFR[0] |= 0x00000200;
    RCC->APB1ENR |= 0x00100000;
    USART5->CR1  &= ~(USART_CR1_UE | USART_CR1_M1| USART_CR1_M0| USART_CR1_OVER8| USART_CR1_PCE);
    USART5->CR2 &= ~(USART_CR2_STOP);
    USART5->BRR = 0x1a1;
    USART5->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;

    while(!((USART5->ISR & USART_ISR_TEACK) && (USART5->ISR & USART_ISR_REACK))){

    }   
}

#ifdef STEP1
int main(void){
    internal_clock();
    init_usart5();
    for(;;) {
        while (!(USART5->ISR & USART_ISR_RXNE)) { }
        char c = USART5->RDR;
        while(!(USART5->ISR & USART_ISR_TXE)) { }
        USART5->TDR = c;
    }
}
#endif

#ifdef STEP2
#include <stdio.h>

// TODO Resolve the echo and carriage-return problem

int __io_putchar(int c) {
    // TODO
    int temp = '\r';
    if(c == '\n'){

        while(!(USART5->ISR & USART_ISR_TXE));
        USART5->TDR = temp;
    }
    while(!(USART5->ISR & USART_ISR_TXE));
    USART5->TDR = c;
    return c;
}

int __io_getchar(void) {
    char temp = '\n';
    while (!(USART5->ISR & USART_ISR_RXNE));
    char c = USART5->RDR;
    if(c == '\r'){
        c = temp;      
    }
    __io_putchar(c);
    // TODO
    return c;
}



int main() {
    internal_clock();
    init_usart5();
    setbuf(stdin,0);
    setbuf(stdout,0);
    setbuf(stderr,0);
    printf("Enter your name: ");
    char name[80];
    fgets(name, 80, stdin);
    printf("Your name is %s", name);
    printf("Type any characters.\n");
    for(;;) {
        char c = getchar();
        putchar(c);
    }
}
#endif

#ifdef STEP3
#include <stdio.h>
#include "fifo.h"
#include "tty.h"
int __io_putchar(int c) {
    // TODO Copy from your STEP2
    int temp = '\r';
    if(c == '\n'){

        while(!(USART5->ISR & USART_ISR_TXE));
        USART5->TDR = temp;
    }
    while(!(USART5->ISR & USART_ISR_TXE));
    USART5->TDR = c;
    return c;
}

int __io_getchar(void) {
    // TODO
    int out;
    out = line_buffer_getchar();
 
    return out;

}

int main() {
    internal_clock();
    init_usart5();
    setbuf(stdin,0);
    setbuf(stdout,0);
    setbuf(stderr,0);
    printf("Enter your name: ");
    char name[80];
    fgets(name, 80, stdin);
    printf("Your name is %s", name);
    printf("Type any characters.\n");
    for(;;) {
        char c = getchar();
        putchar(c);
    }
}
#endif

#ifdef STEP4

#include <stdio.h>
#include "fifo.h"
#include "tty.h"

// TODO DMA data structures
#define FIFOSIZE 16
char serfifo[FIFOSIZE];
int seroffset = 0;

void enable_tty_interrupt(void) {
    // TODO
    USART5->CR1 |= USART_CR1_RXNEIE;
    NVIC->ISER[0] |= 1<< 29;
    USART5->CR3 |= USART_CR3_DMAR;
    RCC->AHBENR |= RCC_AHBENR_DMA2EN;
    DMA2->CSELR |= DMA2_CSELR_CH2_USART5_RX;
    DMA2_Channel2->CCR &= ~DMA_CCR_EN;

    DMA2_Channel2->CMAR = (uint32_t) serfifo;
    DMA2_Channel2->CPAR = (uint32_t) &USART5->RDR;
    DMA2_Channel2->CNDTR = FIFOSIZE;
    DMA2_Channel2->CCR &= ~DMA_CCR_DIR;
    DMA2_Channel2->CCR |= DMA_CCR_MINC;
    
    DMA2_Channel2->CCR &= ~DMA_CCR_MSIZE;
    DMA2_Channel2->CCR &= ~DMA_CCR_PSIZE;
    DMA2_Channel2->CCR |= DMA_CCR_CIRC;
    DMA2_Channel2->CCR |= DMA_CCR_PL;
    DMA2_Channel2->CCR |= DMA_CCR_EN;
}

// Works like line_buffer_getchar(), but does not check or clear ORE nor wait on new characters in USART
char interrupt_getchar() {

    while(fifo_newline(&input_fifo) == 0) {
        asm volatile ("wfi");
    }
    // Return a character from the line buffer.
    char ch = fifo_remove(&input_fifo);
    return ch;
    // TODO
}

int __io_putchar(int c) {
    int temp = '\r';
    if(c == '\n'){

        while(!(USART5->ISR & USART_ISR_TXE));
        USART5->TDR = temp;
    }
    while(!(USART5->ISR & USART_ISR_TXE));
    USART5->TDR = c;
    return c;

}

int __io_getchar(void) {
    // TODO Use interrupt_getchar() instead of line_buffer_getchar()
    int out;
    //out = line_buffer_getchar();
    out = interrupt_getchar();
    return out;
}

// TODO Copy the content for the USART5 ISR here
void USART3_8_IRQHandler(void) {
    while(DMA2_Channel2->CNDTR != sizeof serfifo - seroffset) {
        if (!fifo_full(&input_fifo))
            insert_echo_char(serfifo[seroffset]);
        seroffset = (seroffset + 1) % sizeof serfifo;
    }
}
// TODO Remember to look up for the proper name of the ISR function

int main() {
    internal_clock();
    init_usart5();
    enable_tty_interrupt();

    setbuf(stdin,0); // These turn off buffering; more efficient, but makes it hard to explain why first 1023 characters not dispalyed
    setbuf(stdout,0);
    setbuf(stderr,0);
    printf("Enter your name: "); // Types name but shouldn't echo the characters; USE CTRL-J to finish
    char name[80];
    fgets(name, 80, stdin);
    printf("Your name is %s", name);
    printf("Type any characters.\n"); // After, will type TWO instead of ONE
    for(;;) {
        char c = getchar();
        putchar(c);
    }
}
#endif