
#include "SysTick.h"
#include "ADC.h"
#include <stdio.h>
#include "uart2.h"

void ADC_PISR (void);

__ISR__ SysTick_Handler (void)
{
	//ADC_PISR();
}

void ADC_PISR (void)
{

#define ADC_0_DP0_CHANNEL 0x00



	ADC_Start(ADC0, ADC_0_DP0_CHANNEL, ADC_mA);


}


void SysTick_Init (void)
{
	SysTick->CTRL = 0x00;
	SysTick->LOAD = 12500000L-1; // =125ms
	SysTick->VAL  = 0x00;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}


