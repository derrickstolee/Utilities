/*
 * fraction.cpp
 *
 *  Created on: Apr 17, 2014
 *      Author: stolee
 */

#include "fraction.hpp"
#include <math.h>
#include <numeric>
#include <stdlib.h>
 
/**
 * Shamelessly "borrowed" from Wikipedia:
 * http://en.wikipedia.org/wiki/Binary_GCD_algorithm
 */
unsigned long long int gcd(unsigned long long int u, unsigned long long int v)
{
	long long int shift;

	/* GCD(0,v) == v; GCD(u,0) == u, GCD(0,0) == 0 */
	if ( u == 0 )
		return v;
	if ( v == 0 )
		return u;

	/* Let shift := lg K, where K is the greatest power of 2
	 dividing both u and v. */
	for ( shift = 0; ((u | v) & 1) == 0; ++shift )
	{
		u >>= 1;
		v >>= 1;
	}

	while ( (u & 1) == 0 )
		u >>= 1;

	/* From here on, u is always odd. */
	do
	{
		/* remove all factors of 2 in v -- they are not common */
		/*   note: v is not zero, so while will terminate */
		while ( (v & 1) == 0 ) /* Loop X */
			v >>= 1;

		/* Now u and v are both odd. Swap if necessary so u <= v,
		 then set v = v - u (which is even). For bignums, the
		 swapping is just pointer movement, and the subtraction
		 can be done in-place. */
		if ( u > v )
		{
			unsigned long long int t = v;
			v = u;
			u = t;
		}  // Swap u and v.
		v = v - u;                       // Here v >= u.
	} while ( v != 0 );

	/* restore common factors of 2 */
	return u << shift;
}

unsigned long long int lcm(unsigned long long int u, unsigned long long int v)
{
	return u * (v / gcd(u,v));
}

FractionException::FractionException()
{
}

FractionException::~FractionException()
{
}

const char* FractionException::message()
{
	return "FractionException";
}

OverflowException::OverflowException()
{
}
OverflowException::~OverflowException()
{
}

const char* OverflowException::message()
{
	return "OverflowException";
}

UnderflowException::UnderflowException()
{
}

UnderflowException::~UnderflowException()
{
}

const char* UnderflowException::message()
{
	return "UnderflowException";
}

fraction::fraction()
{
	this->a = 0;
	this->b = 1;
}

fraction::fraction(const fraction& f)
{
	this->a = f.a;
	this->b = f.b;
}

fraction::fraction(int a, int b)
{
	this->a = a;
	this->b = b;
}

fraction fraction::makeFraction(double d, double max_denom)
{
	fraction f;
	f.a = 0;
	f.b = 1;
	double best_approx = 2.0;

	for ( double db = 1.0; db <= max_denom; db++ )
	{
		double da = round(d * db);

		double approx = fabs(d - (da / db));

		if ( approx < best_approx )
		{
			best_approx = approx;
			f.a = (int) da;
			f.b = (int) db;
		}
	}
	return f;
}

fraction fraction::makeFractionFromDenominator(double d, double denom)
{
	fraction f;
	f.a = (int)round(d*denom);
	f.b = (int)denom;
	f.simplify();

	return f;
}


int* global_powers = 0;
fraction fraction::makeFractionFromDivisors(double d, int num_divisors, int* divisors, int* powers, int max_denom)
{
	double best_score = 1.0;
	fraction best_fraction = fraction(-1,0);

	int total_powers = 0;
	for ( int i = 0; i < num_divisors; i++ )
	{
		total_powers += powers[i];
	}

	global_powers = (int*)realloc(global_powers, num_divisors * sizeof(int));
	for ( int i = 0; i < num_divisors; i++ )
	{
		global_powers[i] = -1;
	}

	for ( int power_sum = 1; power_sum <= total_powers; power_sum++ )
	{
		// enumerate all lists majorized by powers
		int cur_depth = 0;
		int cur_pow_sum = 0;
		int divisor = 1;
		
		while ( cur_depth >= 0 )
		{
			// for ( int j = 0; j < cur_depth; j++ )
			// {
			// 	printf(" %d^%d ", divisors[j], global_powers[j]);
			// }
			// printf("\n");

			if ( cur_depth == num_divisors  )
			{
				if ( cur_pow_sum == power_sum && divisor <= max_denom )
				{
					fraction f = fraction::makeFractionFromDenominator(d, divisor);
					double score = fabs(d - (double)f.a/(double)f.b);

					// printf("%d / %d == %lf has score %lf\n", f.a, f.b, (double)f.a/(double)f.b, score);
					if ( score < best_score )
					{
						best_score = score;
						best_fraction = f;
					}
				}

				cur_depth--;
			}
			else if ( cur_pow_sum <= power_sum && divisor <= max_denom && global_powers[cur_depth] < powers[cur_depth] )
			{
				// push!
				global_powers[cur_depth] = global_powers[cur_depth] + 1;

				if ( global_powers[cur_depth] > 0 )
				{
					divisor *= divisors[cur_depth];
					cur_pow_sum++;
				}

				cur_depth++;
			}	
			else
			{
				if ( global_powers[cur_depth] >= 0 )
				{
					// pop!
					for ( int j = 0; j < global_powers[cur_depth]; j++ )
					{
						divisor /= divisors[cur_depth];
					}

					cur_pow_sum -= global_powers[cur_depth];
					global_powers[cur_depth] = -1;
				}

				cur_depth--;
			}
		}	
	}

	return best_fraction;
}

double fraction::bestApproximationFactorFromDivisors(double d, int num_divisors, int* divisors, int* powers, int max_denom)
{
	fraction f = fraction::makeFractionFromDivisors(d,num_divisors,divisors,powers,max_denom);

	return fabs(d - (double)f.a/(double)f.b);
}

double fraction::bestApproximationFactor(double d, double max_denom)
{
	double best_approx = 2.0;

	for ( double db = 1.0; db <= max_denom; db++ )
	{
		double da = round(d * db);

		double approx = fabs(d - (da / db));

		if ( approx < best_approx )
		{
			best_approx = approx;
		}
	}
	return best_approx;
}

double fraction::bestApproximationFactorFromDenominator(double d, double denom)
{
	return fabs( d - (round(d*denom)/denom) );	
}

fraction::~fraction()
{
}

void fraction::simplify()
{
	if ( this->b == 0 )
	{
		this->a = -1;
		return;
	}

	if ( this->a == 0 )
	{
		this->b = 1;
		return;
	}

	if ( this->b < 0 )
	{
		this->b = -this->b;
		this->a = -this->a;
	}

	long long int g = 1;
	if ( this->a > 0 )
	{
		g = (long long int) gcd((unsigned long long int) (this->a), this->b);
	}
	else if ( this->a < 0 )
	{
		g = (long long int) gcd((unsigned long long int) (-this->a), this->b);
	}

	if ( g > 1 )
	{
		this->a = this->a / g;
		this->b = this->b / g;
	}
}

fraction fraction::operator+(fraction f) throw (FractionException)
{
	fraction sum;

	if ( this->b == f.b )
	{
		sum.b = this->b;
		sum.a = this->a + f.a;
	}
	else
	{
		// TODO: check for overflow/underflow here.
		sum.b = this->b * f.b;
		sum.a = (f.b) * this->a + (this->b) * f.a;
	}

	sum.simplify();

	return sum;
}

fraction fraction::operator-(fraction f)
{
	fraction sum;

	if ( this->b == f.b )
	{
		sum.b = this->b;
		sum.a = this->a - f.a;
	}
	else
	{
		// TODO: check for overflow/underflow here.
		sum.b = this->b * f.b;
		sum.a = (f.b) * this->a - (this->b) * f.a;
	}

	sum.simplify();

	return sum;
}

fraction fraction::operator/(fraction f) throw (UnderflowException)
{
	fraction quotient;

	quotient.a = this->a * f.b;
	quotient.b = this->b * f.a;

	quotient.simplify();

	return quotient;
}

fraction fraction::operator*(fraction f) throw (OverflowException)
{
	fraction product;

	product.a = this->a * f.a;
	product.b = this->b * f.b;

	product.simplify();

	return product;
}

bool fraction::operator>(fraction f)
{
	return (this->a * f.b > f.a * this->b);
}

bool fraction::operator>=(fraction f)
{
	return (this->a * f.b >= f.a * this->b);
}

bool fraction::operator<(fraction f)
{
	return (this->a * f.b < f.a * this->b);
}

bool fraction::operator<=(fraction f)
{
	return (this->a * f.b <= f.a * this->b);
}

bool fraction::operator==(fraction f)
{
	this->simplify();
	f.simplify();
	return (this->a == f.a && this->b == f.b);
}
