/*
 * Part1.h
 *
 * Created: 2021-01-23 19:19:01
 *  Author: shirt
 */ 

#include <avr/io.h>
#include <stdint-gcc.h>
#include <stdlib.h>
#include <stdbool.h>

void init(void);

void writeChar(char ch, int pos);

void writeLong(long i);

void print_primes(long i);

int is_prime(long i);