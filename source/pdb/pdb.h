/*
 * pdb.h driver for pdb triggering of ADC
 *
 *  Created on: October 30, 2024
 *      Author: Agustin Casas
 */

#ifndef PDB_H_
#define PDB_H_


/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdbool.h>
#include <stdint.h>
#include "hardware.h"
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define FXOS8700CQ_UPDATE_TICK_MS       20		//50 Hz
#define PISR_PERIOD_MS       20		//50 Hz
#define WRITE_BUFFER_SIZE    20
#define READ_BUFFER_SIZE     20
#define WHOAMI_VALUE         	0xC7




/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef enum {
    PDB_PRESCALER_1,    // 0b000 - Counting uses the peripheral clock divided by 1
    PDB_PRESCALER_2,    // 0b001 - Counting uses the peripheral clock divided by 2
    PDB_PRESCALER_4,    // 0b010 - Counting uses the peripheral clock divided by 4
    PDB_PRESCALER_8,    // 0b011 - Counting uses the peripheral clock divided by 8
    PDB_PRESCALER_16,   // 0b100 - Counting uses the peripheral clock divided by 16
    PDB_PRESCALER_32,   // 0b101 - Counting uses the peripheral clock divided by 32
    PDB_PRESCALER_64,   // 0b110 - Counting uses the peripheral clock divided by 64
    PDB_PRESCALER_128   // 0b111 - Counting uses the peripheral clock divided by 128
} PDB_Prescaler;

typedef enum {
    PDB_MULTIPLIER_1,   // 0b00 - Multiplication factor is 1
    PDB_MULTIPLIER_10,  // 0b01 - Multiplication factor is 10
    PDB_MULTIPLIER_20,  // 0b10 - Multiplication factor is 20
    PDB_MULTIPLIER_40   // 0b11 - Multiplication factor is 40
} PDB_Multiplier;

//Trigger types enum
typedef enum {
    PDB_TRIGG_EXT,
    PDB_TRIGG_CMP0,
    PDB_TRIGG_CMP1,
    PDB_TRIGG_CMP2,
    PDB_TRIGG_PIT0,
    PDB_TRIGG_PIT1,
    PDB_TRIGG_PIT2,
    PDB_TRIGG_PIT3,
    PDB_TRIGG_FTM0,
    PDB_TRIGG_FTM1,
    PDB_TRIGG_FTM2,
    PDB_TRIGG_FTM3,
    PDB_TRIGG_RTC_ALARM,
    PDB_TRIGG_RTC_SECONDS,
    PDB_TRIGG_LPTMR,
    PDB_TRIGG_SW
} pdb_trigger_t;

typedef enum{
	PDB0_CH0,
	PDB0_CH1
}PDB_channel_t;

typedef enum{
	PDB0_PRETRIGGER_A,
	PDB0_PRETRIGGER_B
}PDB_pretrigger_t;

typedef struct{
	PDB_Prescaler prescaler;		//prescaler for the clock according to the PRESCALER enum
	PDB_Multiplier mult;			// muktiplier for the period. the new clock will be (SCLK / (mult * prescaler))
	uint16_t mod;			//the number the counter reaches before the trigger.
	pdb_trigger_t trigger;	//type of trigger for the pdb counter according to pdb_trigger_t
	PDB_channel_t channel;		//PDB channel, 0 or 1 (according to  PDB_channel_t)
	PDB_pretrigger_t pretrigger;
}pdb_config_t;


/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

void pdb_init(bool continuous, bool delay_bypass, pdb_config_t config);

void pdb_set_delay(PDB_channel_t channel, PDB_pretrigger_t pretrigger, uint16_t delay);

void do_software_trigger(void);
/*******************************************************************************
 ******************************************************************************/


#endif /* ACCEL_H_ */
