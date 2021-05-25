
/* Using SPI1 to generate an output on LTC1661
 *
 * SPI1 is configured as master with software slave select.
 * Clock rate is set to 1 MHz. Polarity/Phase are 0, 0.
 * Data frame size is set to 16-bit.
 *
 * PA7  MOSI
 * PA5  SCK
 * PA4  SS
 *
 */

#include "stm32f4xx.h"

void SPI1_init(void);
void DAC_write(short data);

int main(void) {
    int result;
    
    SPI1_init();
  
		/* set up pin PA1 for analog input */
    GPIOA->MODER |=  0xC;           /* PA1 analog */

    /* setup ADC1 */
    RCC->APB2ENR |= 0x00000100;     /* enable ADC1 clock */
    ADC1->CR2 = 0;                  /* SW trigger */
    ADC1->SQR3 = 1;                 /* conversion sequence starts at ch 1 */
    ADC1->SQR1 = 0;                 /* conversion sequence length 1 */
    ADC1->CR2 |= 1;                 /* enable ADC1 */
	
    while(1) {
      ADC1->CR2 |= 0x40000000;        /* start a conversion */
      while(!(ADC1->SR & 2)) {}       /* wait for conv complete */
      result = ADC1->DR;              /* read conversion result */  
			double i = result/4;
      DAC_write((short)(i));      /* write the letter through SPI1 */  
    }
}

/* enable SPI1 and associated GPIO pins */
void SPI1_init(void) {
    RCC->AHB1ENR |= 1;          /* enable GPIOA clock */
    RCC->APB2ENR |= 0x1000;     /* enable SPI1 clock */

    /* PORTA 5, 7 for SPI1 MOSI and SCLK */
    GPIOA->MODER &= ~0x0000CC00;    /* clear pin mode */
    GPIOA->MODER |=  0x00008800;    /* set pin alternate mode */
    GPIOA->AFR[0] &= ~0xF0F00000;   /* clear alt mode */
    GPIOA->AFR[0] |=  0x50500000;   /* set alt mode SPI1 */

    /* PORTA4 as GPIO output for SPI slave select */
    GPIOA->MODER &= ~0x00000300;    /* clear pin mode */
    GPIOA->MODER |=  0x00000100;    /* set pin output mode */
    
    SPI1->CR1 = 0xB1C;              /* set the Baud rate, 16-bit data frame */
    SPI1->CR2 = 0;
    SPI1->CR1 |= 0x40;              /* enable SPI1 module */
}

/* This function enables slave select, writes one byte to SPI1,
   wait for transmit complete and deassert slave select. */
void DAC_write(short data) {
    while (!(SPI1->SR & 2)) {}      /* wait until Transfer buffer Empty */
    GPIOA->BSRR = 0x00100000;       /* deassert slave select */
    SPI1->DR = 0x9000 | ((data << 2) & 0x0FFF);  /* write command and data */
    while (SPI1->SR & 0x80) {}      /* wait for transmission done */
    GPIOA->BSRR = 0x00000010;       /* assert slave select */
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