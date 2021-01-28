/*
 * Lab1.c
 *
 * Created: 2021-01-22 10:29:56
 * Author : shirt
 */ 

#include <avr/io.h>
#include <stdint-gcc.h>
#include <stdlib.h>
#include <stdbool.h>

#define LCDADDRESS 0xEC

// Temporary global data
const int sccDigits[10] = {0x1551, 0x0110, 0x1e11, 0x1B11, 0x0B50, 0x1B41, 0x1F41, 0x0111, 0x1F51, 0x0B51};

void setbits(uint8_t *ptr, int width, int start, uint8_t value)
{
	uint8_t mask = ~(1 << width) << start;
	uint8_t data = (value << start) & mask;
	*ptr = *ptr & ~mask;
	*ptr = *ptr | data;
}


void init(void)
{
	
	CLKPR = 0x80;
	CLKPR = 0x00;
	
	// Using asynchronous clock, setting segments to 25, setting duty to 1/4
	LCDCRB = (1<<LCDCS) | (1<<LCDMUX0) | (1<<LCDMUX1) | (1<<LCDPM2)| (1<<LCDPM1) | (1<<LCDPM0);
	
	// Prescaler set to N = 16, Clock divide D=8
	LCDFRR = (1<<LCDCD2) | (1<<LCDCD1) | (1<<LCDCD0);
	
	//Drive time set to 300 microseconds
	LCDCCR = (1<<LCDCD2) | (1<<LCDCD1) | (1<<LCDCD0);
	
	// Low power waveform, enabling LCD
	LCDCRA = (1<<LCDEN) | (1<<LCDAB);
}

void writeChar(char ch, int pos)
{
	
	if (pos > 5)
	{
		return;
	}
		
	int offset = (pos % 2);
	int column = pos / 2;
	uint8_t mask = 0x0f;
	uint8_t reset_mask;
	uint8_t digit_nibble;
	uint8_t cleared_nibble;
	uint16_t digit;
	uint8_t *addr = (uint8_t*) LCDADDRESS + column;
	
	
	if (isdigit(ch))
	{
		uint8_t index = ch - '0';
		digit = sccDigits[index];
	}
	else
	{
		digit = 0x0000;
	}
	
	for(int i = 0; i < 4; i++)
	{
		reset_mask = (i>0) ? 0b0000 : 0b0110;
		digit_nibble = (digit) & mask;
		cleared_nibble = (*addr << 4*offset) & reset_mask;
		digit_nibble = digit_nibble | cleared_nibble;
		setbits(addr, 4, 4*offset, digit_nibble);
		
		digit = digit >> 4;
		addr+=5;
	}
	
}

void writeLong(long i)
{
	
	long x = i;
	int j = 5;
	char digits[6];
	
	while (x>0 && j >= 0)
	{
		int t = x % 10;
		char ch = t + '0';
		digits[j] = ch;
		j--;
		x = x/10;
	}
	
	for (int i = 0; i < 6; i++)
	{
		writeChar(digits[i],i);
	}
	
}

void print_primes(long i)
{
	while (1)
	{
		if (is_prime(i))
		{
			writeLong(i);
		}
		i++;
	}
}

int is_prime(long i)
{
	long n = i;
	int counter = 0;
	while (n>0)
	{
		if (i%n == 0)
		{
			counter++;
		}
		n--;
	}
	
	if (counter==2)
	{
		return 1;
	}
	return 0;
}


