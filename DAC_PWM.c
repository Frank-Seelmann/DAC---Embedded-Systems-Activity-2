/* Variable duty cycle PWM to control output intensity
 *
 * The change of duty cycle will modulate
 * the intensity of the output.
 * 
 */
 
#include "stm32f4xx.h"

void delayMs(int n);

int main(void) {
    RCC->AHB1ENR |= 7;              /* enable GPIO A, B, and C clock */
  
		int result;
	
    GPIOA->AFR[0] |= 0x00300000;    /* PA5 pin for TIM8 */
    GPIOA->MODER &= ~0x00000C00;
    GPIOA->MODER |=  0x00000800;
    GPIOA->MODER |=  0xC;           /* PA1 analog */
	
		/* setup ADC1 */
    RCC->APB2ENR |= 0x00000100;     /* enable ADC1 clock */
    ADC1->CR2 = 0;                  /* SW trigger */
    ADC1->SQR3 = 1;                 /* conversion sequence starts at ch 1 */
    ADC1->SQR1 = 0;                 /* conversion sequence length 1 */
    ADC1->CR2 |= 1;                 /* enable ADC1 */
	
    /* setup TIM8 */
    RCC->APB2ENR |= 2;              /* enable TIM8 clock */
    TIM8->PSC = 10 - 1;             /* divided by 10 */
    TIM8->ARR = 26667 - 1;          /* divided by 26667 */
    TIM8->CNT = 0;
    TIM8->CCMR1 = 0x0068;           /* PWM mode */
    TIM8->CCER = 4;                 /* enable PWM Ch1N */
    TIM8->CCR1 = 90;                /* pulse width */
    TIM8->BDTR |= 0x8000;           /* enable output */
    TIM8->CR1 = 1;                  /* enable timer */

    while(1) {
				ADC1->CR2 |= 0x40000000;        /* start a conversion */
        while(!(ADC1->SR & 2)) {}       /* wait for conv complete */
        result = ADC1->DR;              /* read conversion result */
				
        TIM8->CCR1 = result * (26667/4096);
        delayMs(5);
    }
}

/* 16 MHz SYSCLK */
void delayMs(int n) {
    int i;

    /* Configure SysTick */
    SysTick->LOAD = 16000;  /* reload with number of clocks per millisecond */
    SysTick->VAL = 0;       /* clear current value register */
    SysTick->CTRL = 0x5;    /* Enable the timer */

    for(i = 0; i < n; i++) {
        while((SysTick->CTRL & 0x10000) == 0) /* wait until the COUNTFLAG is set */
            { }
    }
    SysTick->CTRL = 0;      /* Stop the timer (Enable = 0) */
}