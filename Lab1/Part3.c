#include <avr/io.h>
#include <stdbool.h>

/*
 * Part3.c
 *
 * Created: 2021-01-24 15:11:51
 *  Author: shirt
 */ 


void button(void)
{
	PORTB = PORTB | (1<<7);
	uint8_t prev = 0;
	bool active = false;
	LCDDR0 = 0x6;
	
	while(1)
	{
		uint16_t stick_down = PINB >> 7;
		
		if (stick_down == 0 && prev == 1 && !active)
		{
			LCDDR0 = 0x6;
			LCDDR2 = 0x0;
			prev = 0;
			active = true;
		}
		else if(stick_down == 0 && prev == 0 && !active)
		{
			LCDDR0 = 0x0;
			LCDDR2 = 0x6;
			prev = 1;
			active = true;
		}
		else if (stick_down == 1){
			active = false;
		}
	}
}