#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "include/ZYNQ_driver.h"

#define DEFAULT_PL (const char *) ("/store/mep/zynq_fpga_bin_files/Z_wrapper_atten3.bin")

int get_debug_level(int *debug)
{

	printf("Input mode (1=ERR, 2=DBG, 3=DLOG): ");
	scanf("%d", debug);

	return 0;
}

int get_input_data(uint32_t *data, uint32_t data_mask)
{
	uint32_t lower_word = 0x0000;
	uint32_t upper_word = 0x0000;
	
	if (data_mask & LW_MASK)
	{
		printf("Lower word (16 bits) ");
		scanf("%x", &lower_word);
	}
	
	if (data_mask & UW_MASK) 
	{
		printf("Upper word (16 bits) ");
		scanf("%x", &upper_word);
	}

	data[0] = ( (upper_word & 0x0000ffff) << 16) | (lower_word & 0x0000ffff);
	printf("lower_word=0x%8.8x, upper_word=0x%8.8x, data=0x%8.8x...\n", 
		lower_word, upper_word, data[0]);

	return 0;
}

int get_direction(uint32_t *direction)
{
	uint32_t lower_word;
	uint32_t upper_word;
	
	printf("Lower word direction (16 bits) ");
	scanf("%x", &lower_word);
	printf("Upper word direction (16 bits) ");
	scanf("%x", &upper_word);
	
	*direction = ( (upper_word & 0x0000ffff) << 16) | (lower_word & 0x0000ffff);
	printf("lower_word=0x%8.8x, upper_word=0x%8.8x, direction=0x%8.8x...\n", 
		lower_word, upper_word, *direction);

	return 0;
}

int print_menu (void)
{

	printf("\n\nTest Menu:\n\n");
	printf("Valid key commands\n");
	printf("\tA - Set debug level\n");
	printf("\tB - Get debug level\n");
	printf("\tC - Program PL\n");
	printf("\tD - Initialize Zynq\n");
	printf("\tF - Close Zynq\n");
	printf("\tG - Set GPIO direction\n");
	printf("\tH - Get GPIO direction\n");
	printf("\tI - Write GPIO data\n");
	printf("\tJ - Write GPIO data (lower word)\n");
	printf("\tK - Write GPIO data (upper word)\n");
	printf("\tL - Read GPIO data\n");
	printf("\tQ - Quit...\n");
	printf("\n\tPress any other key to print menu\n");

	return 0;
}

int run_test()
{
	char *fn = "run_test";

	int rv = 0;

	int debug = 0;

	int keepLooping = 1;

	char filename[255];

	char user_input;


	uint32_t direction[MAX_CHANS];

	uint32_t data[MAX_CHANS];

	uint32_t channel_mask;

	direction[0] = 0;
	direction[1] = 0;

	data[0] = 0x00010001;
	data[1] = 0x00000000;

	/* Only using one channel */
	channel_mask = 1;

	/* Use default filename to program the PL */
	sprintf(filename, DEFAULT_PL);

	printf("Starting loop...\n");

	print_menu();

	while (keepLooping)
	{			
			

		while((user_input = getchar()) != '\n' && user_input != EOF)
		{
			switch (user_input)
			{
				case 'a':
				case 'A':

					if ( (rv = get_debug_level(&debug)) != 0)
					{
						printf("%s: ERROR get_debug_mode(%d)\n", fn, debug);
					}

					if ( (rv = zynq_set_debug_level(debug) ) != 0 )
					{
						printf("%s: ERROR setting debug level...\n", fn);
					}

					break;
				
				case 'b':
				case 'B':

					rv = zynq_set_debug_level(debug);
					printf("%s: Debug level=%d...\n", fn, rv);
					
					break;
					
				case 'c':
				case 'C':
			
					printf("%s: Not yet...\n", fn);
					
					break;

				case 'd':
				case 'D':
			
					if ( (rv = zynq_init(OP_TEST_MODE,INIT_OPEN_MODE) ) != 0 )
					{
						printf("%s: ERROR calling zynq_init()...\n", fn);
					}

					printf("%s: zynq_init() done...\n", fn);
					
					break;

				case 'f':
				case 'F':
					
					if ( (rv = zynq_close() ) != 0 )
					{
						printf("%s: ERROR calling zynq_close()...\n", fn);
					}

					printf("%s: zynq_done() done...\n", fn);
			
					break;

				case 'g':
				case 'G':

					if ( (rv = get_direction(direction) ) != 0)
					{
					
						printf("%s: ERROR get_direction(%d)\n", fn, direction[0]);
					}
					
					if ( (rv = zynq_set_gpio_direction(DR, direction, channel_mask) ) != 0 )
					{
						printf("%s: ERROR calling zynq_set_gpio_direction(0x%8.8x)...\n", fn, direction[0]);
					}

					break;

				case 'h':
				case 'H':
				
					if ( (rv = zynq_get_gpio_direction(DR, direction, channel_mask) ) != 0 )
					{
						printf("%s: ERROR calling zynq_get_gpio_direction...\n", fn);
						return -1;
					}
					printf("direction[0]=0x%8.8x, direction[1]=0x%8.8x...\n", direction[0], direction[1]);
			
					break;


				case 'i':
				case 'I':		
					if ( (rv = get_input_data(data, UW_MASK | LW_MASK) ) != 0)
					{
						printf("%s: ERROR get_input_data(%d)\n", fn, data[0]);
					}

					if ( (rv = zynq_write(DR, data, channel_mask) ) != 0 )
					{
						printf("%s: ERROR calling zynq_write(0x%8.8x)...\n", fn, data[0]);
					}
		
					break;

				case 'j':
				case 'J':
					
					if ( (rv = get_input_data(data, LW_MASK) ) != 0)
					{
						printf("%s: ERROR get_input_data(%d)\n", fn, data[0]);
					}

					if ( (rv = zynq_write_lw(DR, data, channel_mask) ) != 0 )
					{
						printf("ERROR calling zynq_write(0x%8.8x)...\n", data[0]);
					}
		
					break;

				case 'k':
				case 'K':
					
					if ( (rv = get_input_data(data, UW_MASK) ) != 0)
					{
						printf("%s: ERROR get_input_data(%d)\n", fn, data[0]);
					}

					if ( (rv = zynq_write_uw(DR, data, channel_mask) ) != 0 )
					{
						printf("ERROR calling zynq_write(0x%8.8x)...\n", data[0]);
					}
		
					break;

				case 'l':
				case 'L':

					if ( (rv = get_direction(direction) ) != 0)
					{
						printf("%s: ERROR get_direction(%d)\n", fn, data[0]);
					}
					
					if ( (rv = zynq_get_gpio_direction(DR, direction, channel_mask) ) != 0 )
					{
						printf("%s: ERROR calling zynq_get_gpio_direction...\n", fn);
						return -1;
					}
					printf("direction[0]=0x%8.8x, direction[1]=0x%8.8x...\n", direction[0], direction[1]);
			
					break;

				case 'q':
				case 'Q':

					if ( (rv = zynq_close() ) != 0 )
					{
						printf("%s: ERROR calling zynq_close()...\n", fn);
					}
					printf("%s: Leaving loop...\n", fn);
					keepLooping = 0; 
					break;

				default:

					printf("%s: Invalid input\n", fn);
					break;

			}
	
			/* Only re-print menu if need to pass through loop cycle */	
			if (keepLooping)
			{
				print_menu();
			}
		
		}
	}

	return 0;

}

int main()
{

	int rv = 0;

	printf("Running test...\n");
	
	if ( (rv = run_test()) != 0)
	{
		printf("ERROR running test...\n");
	}

	printf("Done...\n");

	return 0;

}
