/* Use DAC to generate output.
 * The DAC is initialized with no buffer or trigger, so every 
 * write to the DAC data register will change the analog output.
 * The output of DAC is on pin PA4.
 */
#include "stm32f4xx.h"

int main(void) {
	
    int result;
		
    RCC->AHB1ENR |=  1;	            /* enable GPIOA clock */    
    GPIOA->MODER |=  0x00000300;    /* PA4 analog */
    /* setup DAC */
    RCC->APB1ENR |= 1 << 29;        /* enable DAC clock */
    DAC->CR |= 1;                   /* enable DAC */
		
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
					
      DAC->DHR12R1 = result;
    }
}