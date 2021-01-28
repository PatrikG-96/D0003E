/*
 * Part4.c
 *
 * Created: 2021-01-28 21:42:42
 *  Author: shirt
 */ 

#include "utils.h"
#include <stdbool.h>
#include <stdint-gcc.h>
#include <avr/io.h>


typedef struct stick_state {
	bool stick_active;		// stays true while the joystick is in its down state
	uint8_t stick_prev;		// keeps track of previous active lcd segment
} stick_state;

typedef struct timer_state {
	uint16_t next_value;	// next value to wait for
	uint16_t timer_step;	// value to increment next value with
	bool lcd_active;		// current state of the lcd segment
	bool timer_wrapping;	// whether or not we need to wait for timer to wrap
}timer_state;

void init_All()
{
	initLCD();
	TCCR1B = (1<<CS12);
	PORTB = PORTB | (1<<7);
}

void blink2(timer_state* state)
{

	if (TCNT1 >= state->next_value && !state->timer_wrapping)
	{
		
		state->lcd_active = !state->lcd_active;	// alternate states
		state->next_value += state->timer_step;
		
		if (state->next_value >= 0xFFFF)
		{
			state->next_value -= 0xFFFF;	// make sure wrapping doesn't affect timer
		}
		
		if (TCNT1 > state->next_value) // if the next timer value is greater than TCNT1, we need to wait for timer to wrap
		{
			state->timer_wrapping = true;
		}
		
		if (state->lcd_active)
		{
			setbits(&LCDDR3, 1, 0, 0x1);
		}
		else
		{
			setbits(&LCDDR3, 1, 0, 0x0);
		}
	}

	if (TCNT1 < state->next_value)
	{
		state->timer_wrapping = false; // if next value > TCNT1, we don't need to wait for wrapping
	}
}

void button2(stick_state* state)
{
	uint16_t stick_down = PINB >> 7;
	
	// If joystick is currently in a down state and wasn't before
	if (stick_down == 0 && state->stick_prev == 1 && !state->stick_active)
	{
		setbits(&LCDDR0, 2, 1, 0x3);
		setbits(&LCDDR2, 2, 1, 0x0);
		state->stick_prev = 0;
		state->stick_active = true;
	}
	else if(stick_down == 0 && state->stick_prev == 0 && !state->stick_active)
	{
		setbits(&LCDDR0, 2, 1, 0x0);
		setbits(&LCDDR2, 2, 1, 0x3);
		state->stick_prev = 1;
		state->stick_active = true;
	}
	// If stick is currently not in a down state
	else if (stick_down == 1){
		state->stick_active = false;
	}
}

int run_all(long start)
{
	//return 

	stick_state stick = {false, 0};
	timer_state timer = {0, 8000000/512, false, false};
	long i = start;
	
	while(1)
	{
		
		blink2(&timer);
		
		if (is_prime(i))
		{
			writeLong(i);
		}
		//writeChar('3',5);
		button2(&stick);
		
		i++;
	}
}