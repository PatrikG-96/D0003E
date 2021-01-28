#include "Part1.h"
#include "Part2.h"
#include "Part3.h"
#include "Part4.h"


int main()
{
	return 0;
	int n = 4;
	init_All();
	while(1)
	{
		switch (n)
		{
			case 1:
			primes(1);
			break;
			case 2:
			blink();
			break;
			case 3:
			button();
			break;
			case 4:
			run_all(25000);
			break;
		}
	}

}