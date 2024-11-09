#include "demod.h" 
#include "ADC.h"


void add_data(uint16_t new_data);
void data_fsm(uint8_t data_in);

static const float coefs[FILTER_SIZE] = {
		 -0.002086994263159,-0.006003054525357,-0.008633170414909,-0.001790251586214,
		    0.02460882704212,   0.0756853298976,   0.1448247875714,   0.2127701317527,
		     0.2551960820215,   0.2551960820215,   0.2127701317527,   0.1448247875714,
		     0.0756853298976,  0.02460882704212,-0.001790251586214,-0.008633170414909,
		  -0.006003054525357,-0.002086994263159
		};

static float raw_inputs[MULT_BUFF_SIZE];
static uint8_t write_index_raw;



static float inputs[BUFFER_SIZE];
static uint8_t* demod_bitstream;

static uint8_t write_index;
static uint8_t read_index;

static uint16_t demod_write_index;		//


static uint8_t current_state;			//State of the hysteresis trigger
static uint16_t unfiltered_data_count;	//number of data samples that were not filtered

static COMM_STATE_t stream_state;	//State of the transmission according to the enum COMM_STATE_t.
static uint8_t sample_count;			//counter for the number of samples in a bit.
static uint8_t bit_count;			//counter of bits in a message to find the end bit.
static uint8_t zero_count;			//counter for the amount of 0s in the start bit
static uint8_t parity;				//variable to calculate parity.
static uint8_t error_flag;			//Error from parity or lack of end bit.
static uint8_t data_byte;			//data byte demodulated
static bool data_ready;				//flag that indicates data is ready to be read.


void demod_init( uint8_t* demod_bitstream_ptr){
	setADCIRQ(add_data);
	demod_bitstream = demod_bitstream_ptr;
}

uint8_t filter_data (void){
#ifdef DEBUG_PIN
	DEBUG_TP_SET;
	//GPIO_Set(PTC, GPIO_OUT(12));
#endif

	int i,j;
	uint8_t num_data=0;			//number of data samples filtered
	float filtered_data=0;
	if(write_index >= read_index){
		num_data= write_index-read_index;
	}
	else{
		num_data= write_index - read_index + (BUFFER_SIZE);
	}
	for(j=0; j<num_data; j++)
	{
		for(i=0; i<FILTER_SIZE; i++){
			filtered_data += inputs[CIRCULAR_INDEX_R(read_index - i, BUFFER_SIZE)] * coefs[i];
		}
		read_index = UPDATE_INDEX(read_index,BUFFER_SIZE);

		if(filtered_data > V_TH){
				current_state = LOGIC_0;	// Set to 0 when amplitude is greater than V_th
		}
		else if( filtered_data < V_TL){
				current_state = LOGIC_1;	//Set to 1 when amplitude is less than V_tl
		}
		data_fsm(current_state);

		demod_bitstream[demod_write_index]= current_state;						//load data into array
		demod_write_index = UPDATE_INDEX(demod_write_index, BITSTREAM_SIZE);	//update write index

		unfiltered_data_count--;
		filtered_data=0;			//restart the aux variable.
	}
	#ifdef DEBUG_PIN
		DEBUG_TP_CLR;
		//GPIO_Clear(PTC, GPIO_OUT(12));
	#endif
	return num_data;
}

void add_data(uint16_t new_data){

	raw_inputs[write_index_raw] = (new_data-2048)*(3.3f)/4095;
	inputs[write_index] = raw_inputs[write_index_raw] * raw_inputs[CIRCULAR_INDEX_R(write_index_raw-(MULT_BUFF_SIZE-1), MULT_BUFF_SIZE)];

	write_index_raw = UPDATE_INDEX(write_index_raw ,MULT_BUFF_SIZE);
	write_index = UPDATE_INDEX(write_index,BUFFER_SIZE);
	unfiltered_data_count++;
}


uint16_t get_unfiltered_count(void){
	return unfiltered_data_count;
}

bool is_data_ready(void){
	return data_ready;
}

uint8_t get_data_byte(void){
	return data_byte;
}

void data_fsm(uint8_t data_in){
	switch(stream_state){
	case IDLE:
		if(data_in == LOGIC_0){
			stream_state = CHECK_START;
			zero_count++;
			sample_count++;
		}
		break;
	case CHECK_START:
		sample_count++;
		if((sample_count < 6) && (data_in == LOGIC_0)){
			zero_count++;
		}
		else if (sample_count == OVERSAMPLE_RATE){
			if(zero_count > 2){
				stream_state = RECIEVING;	//mayority of the sampled bits are 0. start is considered
				error_flag=0;
				data_byte =0;
				data_ready = false;
			}
			else{
				stream_state = IDLE;		//considered a false start
			}
			zero_count=0;			//reset counters
			sample_count =0;
			parity =0;				//reset parity
			bit_count = 0;			//make sure bit_count is in 0
		}

		break;

	case RECIEVING:
		sample_count++;
		if((sample_count > COMP_WINDOW_LOW) && (sample_count < COMP_WINDOW_HIGH)){		//take the 5 middle samples
			if(data_in == LOGIC_0){
				zero_count++;
			}
		}
		else if(sample_count == OVERSAMPLE_RATE){
			//data byte is unchanged IF 0 IS RECIEVED!!!
			//if(zero_count > COMP_THRESHOLD){
				//data_byte += ( 0 << bit_count);
				//parity ^= LOGIC_0;
			//}
			if(zero_count <= COMP_THRESHOLD){
				data_byte += (1<<bit_count);			//UART IS LSB FIRST. SHIFT THE DATA BIT N BITS AND ADD IT
				parity ^= LOGIC_1;
			}
			zero_count =0;			//Restart counters
			sample_count=0;
			bit_count++;		//add 1 to bit count
		}

		if(bit_count == INFO_BIT_NUM){		//if 8 bits of data have been read
			bit_count=0;
			stream_state = PARITY;
		}

		break;

	case PARITY:
		sample_count++;
		if((sample_count > COMP_WINDOW_LOW) && (sample_count < COMP_WINDOW_HIGH)){		//take the 5 middle samples
			if(data_in == LOGIC_0){
				zero_count++;
			}
		}
		else if(sample_count == OVERSAMPLE_RATE){
			//XOR with 0 doesn't change the result. Only analyze for 1
			if(zero_count <= COMP_THRESHOLD){
				parity ^= LOGIC_1;
			}

			if(parity == PARITY_VALUE){
				error_flag =NO_ERR;				//check for parity
			}
			else{
				error_flag =PARITY_ERR;
			}

			zero_count =0;			//Restart counters
			sample_count=0;
			stream_state = END;
		}

		break;
	case END:
		sample_count++;
		if((sample_count > COMP_WINDOW_LOW) && (sample_count < COMP_WINDOW_HIGH)){		//take the 5 middle samples
			if(data_in == LOGIC_0){
				zero_count++;
			}
		}
		else if(sample_count == OVERSAMPLE_RATE){
			if(zero_count > COMP_THRESHOLD){			//END BIT SHOULD BE 1.
				error_flag += END_ERR;					//By adding, if there was a parity error it will still show, because the flag will take the value of BOTH_ERR
			}
			zero_count =0;			//Restart counters
			sample_count=0;
			stream_state = IDLE;
			if(error_flag == NO_ERR){
				data_ready = true;
			}
		}
		break;

	default:

	}
}

