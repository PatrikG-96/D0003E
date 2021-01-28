#include "Part1.h"
#include "Part2.h"
#include "Part3.h"


struct stick_state {
	bool stick_active;
	uint8_t stick_prev;
	} stick_state;

struct timer_state {
	uint16_t next_value;
	uint16_t timer_step;
	bool lcd_active;
	bool timer_wrapping;
	}timer_state;

void init_All()
{
	init();
	TCCR1B = (1<<CS12);
	PORTB = PORTB | (1<<7);
}

void blink2(struct timer_state* state)
{
	uint8_t *addr = &LCDDR3;
	if (TCNT1 >= state->next_value && !state->timer_wrapping)
	{
		
		state->lcd_active = !state->lcd_active;	// alternate states
		state->next_value += state->timer_step;
		
		// If timer goes beyond TCNT1 maximum value of 0xffff, we need to decrement current timer with 0xffff as a means of resetting
		// without losing the progress to the next trigger
		if (state->next_value >= 0xFFFF)
		{		
			state->next_value -= 0xFFFF;
		}
		
		// If after decrementing timer by 0xFFFF TCNT1 is larger, we need to wait for TCNT1 to wrap
		if (TCNT1 > state->next_value)
		{
			state->timer_wrapping = true;
		}
		
		if (state->lcd_active)
		{
			setbits(addr, 1, 0, 0b1);
		}
		else
		{
			setbits(addr, 1, 0, 0b0);
		}
	}
	// If timer > TNCT1 we no longer need to wait for wrapping
	if (TCNT1 < state->next_value)
	{
		state->timer_wrapping = false;
	}
}

void button2(struct stick_state* state)
{
	uint16_t stick_down = PINB >> 7;
	
	if (stick_down == 0 && state->stick_prev == 1 && !state->stick_active)
	{
		setbits(&LCDDR0, 2, 1, 0b11);
		setbits(&LCDDR2, 2, 1, 0b00);
		state->stick_prev = 0;
		state->stick_active = true;
	}
	else if(stick_down == 0 && state->stick_prev == 0 && !state->stick_active)
	{
		setbits(&LCDDR0, 2, 1, 0b00);
		setbits(&LCDDR2, 2, 1, 0b11);
		state->stick_prev = 1;
		state->stick_active = true;
	}
	else if (stick_down == 1){
		state->stick_active = false;
	}
}

int main()
{
	init_All();
	
	struct stick_state stick = {false, 0};
	struct timer_state timer = {0, 8000000/512, false, false};
	long i = 25000;
	
	while(1)
	{
		return 0;	
		//LCDDR2 = 0b00100000;
		blink2(&timer);
		//
		if (is_prime(i))
		{
			writeLong(i);
		}
	
		//writeLong(i);
		//writeLong(i);
		
 		button2(&stick);
		
		i++;
	}
}