/*
 * test_fraction.cpp
 *
 *  Created on: Apr 18, 2014
 *      Author: dstolee
 */

#include "fraction.hpp"

#include <stdio.h>

void print(fraction f)
{
	printf("%lld / %lld ", f.a, f.b);
}

int main(void)
{
	fraction f1, f2, f3;

	f1.a = 3;
	f1.b = 5;
	f2.a = 5;
	f2.b = 3;

	f3 = f1 + f2;
	print(f1);
	printf(" + ");
	print(f2);
	printf(" = ");
	print(f3);
	printf("\n");

	f3 = f1 - f2;
	print(f1);
	printf(" - ");
	print(f2);
	printf(" = ");
	print(f3);
	printf("\n");

	f3 = f1 / f2;
	print(f1);
	printf(" / ");
	print(f2);
	printf(" = ");
	print(f3);
	printf("\n");

	f3 = f1 * f2;
	print(f1);
	printf(" * ");
	print(f2);
	printf(" = ");
	print(f3);
	printf("\n");

	f1.a = 12;
	f1.b = 5;
	f2.a = 7;
	f2.b = 15;

	f3 = f1 + f2;
	print(f1);
	printf(" + ");
	print(f2);
	printf(" = ");
	print(f3);
	printf("\n");

	f3 = f1 - f2;
	print(f1);
	printf(" - ");
	print(f2);
	printf(" = ");
	print(f3);
	printf("\n");

	f3 = f1 / f2;
	print(f1);
	printf(" / ");
	print(f2);
	printf(" = ");
	print(f3);
	printf("\n");

	f3 = f1 * f2;
	print(f1);
	printf(" * ");
	print(f2);
	printf(" = ");
	print(f3);
	printf("\n");

	f1.a = -1;
	f1.b = 24;
	f2.a = 3;
	f2.b = 58;

	f3 = f1 + f2;
	print(f1);
	printf(" + ");
	print(f2);
	printf(" = ");
	print(f3);
	printf("\n");

	f3 = f1 - f2;
	print(f1);
	printf(" - ");
	print(f2);
	printf(" = ");
	print(f3);
	printf("\n");

	f3 = f1 / f2;
	print(f1);
	printf(" / ");
	print(f2);
	printf(" = ");
	print(f3);
	printf("\n");

	f3 = f1 * f2;
	print(f1);
	printf(" * ");
	print(f2);
	printf(" = ");
	print(f3);
	printf("\n");

	f1 = fraction::makeFraction(3.14159, 50);
	f2 = fraction::makeFraction(2.71829, 50);
	f3 = f1 * f2;
	print(f1);
	printf(" * ");
	print(f2);
	printf(" = ");
	print(f3);
	printf("\n");

	int num_primes = 10;

	int* primes = new int[num_primes];
	primes[0] = 2;
	primes[1] = 3;
	primes[2] = 5;
	primes[3] = 7;
	primes[4] = 11;
	primes[5] = 13;
	primes[6] = 17;
	primes[7] = 19;
	primes[8] = 23;
	primes[9] = 29;

	int* powers = new int[num_primes];
	powers[0] = 8;
	powers[1] = 5;
	powers[2] = 3;
	powers[3] = 2;
	powers[4] = 1;
	powers[5] = 1;
	powers[6] = 1;
	powers[7] = 1;
	powers[8] = 1;
	powers[9] = 1;


	printf("Testing divisors.\n");
	f1 = fraction::makeFractionFromDivisors(0.4181818, 6, primes, powers, 1000);
	f2 = fraction::makeFractionFromDivisors(2.71829,  6, primes, powers, 1000);
	f3 = f1 * f2;
	print(f1);
	printf(" * ");
	print(f2);
	printf(" = ");
	print(f3);
	printf("\n");



}
