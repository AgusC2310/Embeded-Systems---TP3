/*
 * main.c
 *
 *  Created on: May 1, 2015
 *      Author: Juan Pablo VEGA - Laboratorio de Microprocesadores
 */

#include "hardware.h"
#include "PORT.h"
#include "gpio/gpio.h"
#include "FTM.h"
#include "uart2.h"
#include "ADC.h"
#include "SysTick.h"
#include <stdio.h>
#include "serial/serial.h"
#include "./pdb/pdb.h"
#include <stdbool.h>
#include "demod.h"
#define __FOREVER__ 	for(;;)


ADCData_t data;
float dato;
uint32_t cont;
pdb_config_t conf;

uint8_t bitstream[1000];
uint16_t filt_cnt;

int main (void)
{
 	 	 	 	hw_Init();
 	 	 	 	PORT_Init();
 	 	 		//GPIO_Init();
 	 	 		serial_init(250000);
 	 	 		//SysTick_Init();		//No quiero por software
			#ifdef DEBUG_PIN
 	 	 		//GPIO_SetDirection(PTC, 12, GPIO_OUT(12));
 	 	 		gpioMode(PORTNUM2PIN(PB,DEBUG_PIN), OUTPUT);
			#endif
			#ifdef DEBUG_PIN12C
					//GPIO_SetDirection(PTC, 12, GPIO_OUT(12));
					gpioMode(PORTNUM2PIN(PC,DEBUG_PIN12C), OUTPUT);
			#endif
 	 	 		//settings for sample freq of 12kHz
 	 	 		conf.mod = 2083U;
 	 	 		conf.mult = PDB_MULTIPLIER_1;
 	 	 		conf.prescaler = PDB_PRESCALER_2;

 	 	 		conf.trigger = PDB_TRIGG_SW;
 	 	 		conf.channel = PDB0_CH0;
 	 	 		conf.pretrigger = PDB0_PRETRIGGER_A;
 	 	 		pdb_init(1,0, conf);
 	 	 		pdb_set_delay(PDB0_CH0, PDB0_PRETRIGGER_A, 2);
 	 	 		demod_init(bitstream);
 	 	 		ADC_Init(true);

 	 	 		do_software_trigger();
 	 	 		//ADC_Calibrate(ADC0);
				serial_print("A");
				serial_print("B");
				serial_print("C");



 	 	// 		hw_DisableInterrupts();

 	 	 		__FOREVER__{
					cont = get_unfiltered_count();
 	 	 				if(cont >= 2){
 	 	 					filter_data();
 	 	 				}
 	 	 			}

			// Enable interrupts
			//hw_EnableInterrupts();

}

