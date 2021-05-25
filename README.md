# DAC---Embedded-Systems-Activity-2

Introduction

For this activity, students were tasked with designing three systems in order to verify and
compare the operations of three digital to analog conversion methods. These methods are:
Pulse Width Modulation, an on-chip Digital to Analog Converter, and an SPI based DAC
called an LTC1661. Each of the three systems are meant to use the analog to digital
converter on the microcontroller to take a varying voltage as input, then output to a motor
using its DAC methodology.

Design

Due to the limited current output of the NUCLEO board, none of the DAC methods were
able to drive the motor directly, even at their highest output. To remedy this problem a
ULN2003 Darlington Transistor Array was used. In short, the setup allows a battery to power
the motor, which is connected to the transistor output, but the DAC method, connected to
the transistor input, to determine how fast it should go. 

The input value determining what the output should be for each of the digital to analog 
strategies is an ADC input determined by a potentiometer. The ADC on the microcontroller 
has 12 bits of resolution, so that means that there are 2^12 or 4096 steps (where a step
is the smallest change discernible by the ADC). This input result can be converted, or 
mapped, to a proportional step for each DAC strategy. 

The PWM program configures TIM8, a 16-bit timer, to count at  1.6MHz. For this setup, the 
frequency is determined by TIM8 ARR, and the duty cycle by TIM8 CCR1. Since it is known 
that the range for the duty cycle is 0 to 26667 and the range of the result of the ADC is
4096, the ADC result can be mapped to the PWM by multiplying by 26667/4096 and passing it 
to TIM8 CCR1, which outputs to PA5.

The on-chip DAC program enables the DAC for 12 bits of resolution. Since the ADC
also has 12 bits of resolution, the value from the ADC result (0 to 4096) is 1:1 with the
DAC, so it can be passed directly to the DAC DHR12R1, which outputs to PA4.

Results

In theory all three strategies should cause the motor to spin at an increasing rate as the
potentiometer is turned. As shown in the verification video, https://www.youtube.com/watch?v=pnMbeEv8uSk&t=20s,
only PWM was able to achieve this for the full range of the potentiometer’s motion. For both 
the on-chip DAC and the LTC1661, the motor only began spinning after the potentiometer was 
turned about half way, after which point it continues to increase in speed as the potentiometer 
is turned.

An interesting behaviour for the PWM method was that when the potentiometer is turned
all the way to the off position, the output jumps around rather than staying at 0. This is
shown with an LED in the verification video.

All three DAC methods showed no discernible difference in the max speed of the motor
when the potentiometer is turned to its maximum angle.

Conclusion

After verifying the operation of all three digital to analog methodologies and comparing
them, we are able to make a more informed decision as to which one to use for future
projects.

Of the three, PWM gives the best results, as it was the only one to function as anticipated
for the whole potentiometer range. The only problem it had was that it was giving some
small output when it was supposed to have no output. This can be fixed by simply cutting
off the output if the ADC input is low enough, if it cannot be ignored. Both the on chip
DAC and the SPI based DAC were less desirable as they needed to reach an input
threshold before providing an output, and as such were not able to take advantage of the
full range of motion of the input potentiometer
