#ifndef DEMOD_H
#define DEMOD_H

#include <stdint.h>
#define FILTER_SIZE 18
#define BUFFER_SIZE 100
#define BITSTREAM_SIZE 1000
#define MULT_BUFF_SIZE 6
#define OVERSAMPLE_RATE 10
#define INFO_BIT_NUM 8

#define COMP_WINDOW_LOW 3
#define COMP_WINDOW_HIGH 9
#define COMP_THRESHOLD 2		//number of middle samples must be GREATER than the threshold to be considered valid.

#define LOGIC_0 0
#define LOGIC_1 1
#define PARITY_VALUE 1		// 1 for odd parity, 0 for even.

//macro for circular buffer index update. ind = previous index. size = number of elements in the array. returns 0 when ind==(size-1)
#define UPDATE_INDEX(ind, size) (((ind)==((size)-1)) ? 0 : (ind+1))

//macro for circular buffer index range correction. DO NOT USE WITH IND>=2*SIZE.
#define CIRCULAR_INDEX_W(ind, size) (((ind)>=(size)) ? ((ind)-(size)) : (ind))


#define CIRCULAR_INDEX_R(ind, size) (((ind)<(0)) ? ((ind)+(size)) : (ind))

#define V_TH 0.1f
#define V_TL -0.2f


typedef enum {
	IDLE,
	CHECK_START,
	RECIEVING,
	PARITY,
	END,
}COMM_STATE_t;

typedef enum{
	NO_ERR,
	PARITY_ERR,
	END_ERR,
	BOTH_ERR
}DEMOD_ERROR_t;


void demod_init( uint8_t* demod_bitstream_ptr);

uint8_t filter_data (void);

uint16_t get_unfiltered_count(void);

#endif
