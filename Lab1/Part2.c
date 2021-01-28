#include <stdbool.h>
#include <avr/io.h>
#include <stdint-gcc.h>


/*
 * Part2.c
 *
 * Created: 2021-01-23 19:22:22
 *  Author: shirt
 */ 


void blink(void)
{
	// Page 120 Table 59, prescale of N=256
	TCCR1B = (1<<CS12);
	uint16_t timer = 0;
	uint16_t timerStep = 8000000/512;
	bool active = false;
	bool wrapping = false;

	while(1)
	{
		if (TCNT1 >= timer && !wrapping)
		{
			active = !active;	// alternate states
			timer += timerStep;
			
			// If timer goes beyond TCNT1 maximum value of 0xffff, we need to decrement current timer with 0xffff as a means of resetting 
			// without losing the progress to the next trigger
			if (timer >= 0xFFFF)
			{
				timer -= 0xFFFF;	
			}
			
			// If after decrementing timer by 0xFFFF TCNT1 is larger, we need to wait for TCNT1 to wrap  
			if (TCNT1 > timer)
			{
				wrapping = true;
			}
			
			set_segment(active);		
		}
		// If timer > TNCT1 we no longer need to wait for wrapping
		if (TCNT1 < timer)
		{
			wrapping = false;
		}
	}
}


void set_segment(bool active)
{
	if (active)
	{
		LCDDR3 = 0x0;
	}
	else
	{
		LCDDR3 = 0x1;
	}
}