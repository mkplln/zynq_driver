#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "include/ZYNQ_driver.h"

#define DEFAULT_PL (const char *) ("/store/mep/zynq_fpga_bin_files/Z_wrapper_atten3.bin")

int main()
{

	int rv = 0;
	
	int debug = 0;

	char filename[255];

	int opmode = OP_TEST_MODE;

	int initmode = INIT_OPEN_MODE; 

	uint32_t direction[MAX_CHANS];

	uint32_t data[MAX_CHANS];

	uint32_t channel_mask;

	direction[0] = 0;
	direction[1] = 0;

	data[0] = 0x00010001;
	data[1] = 0x00000000;

	channel_mask = 1;

	sprintf(filename, DEFAULT_PL);

	if ( (rv = zynq_set_debug_level(debug) ) != 0 )
	{
		printf("ERROR setting debug level...\n");
	}


	if ( (rv = zynq_init(opmode, initmode) ) != 0 )
	{
		printf("ERROR calling zynq_init()...\n");
	}


	if ( (rv = zynq_set_gpio_direction(DR, direction, channel_mask) ) != 0 )
	{
		printf("ERROR calling zynq_set_gpio_direction(0x%8.8x)...\n", direction[0]);
	}

	if ( (rv = zynq_get_gpio_direction(DR, direction, channel_mask) ) != 0 )
	{
		printf("ERROR calling zynq_get_gpio_direction...\n");
	}

	printf("direction[0]=0x%8.8x, direction[1]=0x%8.8x...\n", direction[0], direction[1]);

	if ( (rv = zynq_set_gpio_direction(DR, direction, channel_mask) ) != 0 )
	{
		printf("ERROR calling zynq_set_gpio_direction(0x%8.8x)...\n", direction[0]);
	}

	if ( (rv = zynq_get_gpio_direction(DR, direction, channel_mask) ) != 0 )
	{
		printf("ERROR calling zynq_get_gpio_direction...\n");
	}

	printf("direction[0]=0x%8.8x, direction[1]=0x%8.8x...\n", direction[0], direction[1]);


	printf("Starting loop...\n");

	while (1)
	{
		data[0] = 0x00010001;
		data[1] = 0x00000000;

		if ( (rv = zynq_write(DR, data, channel_mask) ) != 0 )
		{
			printf("ERROR calling zynq_write(0x%8.8x)...\n", data[0]);
		}

		data[0] = 0x00000000;
		data[1] = 0x00000000;

		if ( (rv = zynq_write(DR, data, channel_mask) ) != 0 )
		{
			printf("ERROR calling zynq_write(0x%8.8x)...\n", data[0]);
		}
	
	}

	if ( (rv = zynq_close() ) != 0 )
	{
		printf("ERROR calling zynq_close()...\n");
	}

}
