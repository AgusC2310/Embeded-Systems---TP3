/*
 * Author: HOUSES
 */

#include "pdb.h"
#include "../../CMSIS/MK64F12.h"

static PDB_Type* const pdb_ptr = PDB0;		//ptr to the PDB registers

static char flagerr = 0;

void pdb_init(bool continuous, bool delay_bypass, pdb_config_t config){

	SIM->SCGC6 |= SIM_SCGC6_PDB_MASK;	//clock gating!!!
	NVIC_EnableIRQ(PDB0_IRQn);

	pdb_ptr->SC |= PDB_SC_CONT(continuous);	//enable continuous mode for ADC conversion to be periodic

	//Clock scaling
	pdb_ptr->SC |= PDB_SC_MULT(config.mult);// multiplier for clock
	pdb_ptr->SC |= PDB_SC_PRESCALER(config.prescaler);	//prescaler for clock

	pdb_ptr->MOD &= PDB_MOD_MOD(config.mod);	//count up to mod.

	pdb_ptr->SC |= PDB_SC_PDBEN(0X01);			//enable pdb!!!!!!

	pdb_ptr->SC |= PDB_SC_TRGSEL(config.trigger);	//select trigger
	pdb_ptr->SC |= PDB_SC_PDBIE(0X01);				//enable interrupts for error debugging

	if(!delay_bypass){
		pdb_ptr->CH[config.channel].C1 |= PDB_C1_TOS(0X1); //dont bypass the delay
	}
	//IMPORTANT
	pdb_ptr->CH[config.channel].C1 = PDB_C1_EN((config.pretrigger) + 1 );	//enable pre triggers. 0b01 is for pretrigger A and 0b10 for B.

	pdb_ptr->SC |= PDB_SC_LDOK(0x01);	//UPDATE VALUES FROM THEIR BUFFERS!!!!!!!!!!!

}
void do_software_trigger(void){
	pdb_ptr->SC |= PDB_SC_SWTRIG(0x01);		//Software trigger to start PDB when it is in continuous mode
}

void pdb_set_delay(PDB_channel_t channel, PDB_pretrigger_t pretrigger, uint16_t delay){
	pdb_ptr->CH[channel].DLY[pretrigger] = PDB_DLY_DLY(delay);
	pdb_ptr->CH[channel].C1 |= PDB_C1_TOS(0X1); //dont bypass the delay

	pdb_ptr->SC |= PDB_SC_LDOK(0x01);	//UPDATE VALUES FROM THEIR BUFFERS!!!!!!!!!!!!
}

__ISR__ PDB0_IRQHandler (void){
	flagerr=1;
	pdb_ptr->CH[0].S = 0x0;
}
