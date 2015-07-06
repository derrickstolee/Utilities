/***********************************************************

 Copyright Derrick Stolee 2011.

 This file is part of SearchLib.

 SearchLib is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 SearchLib is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with SearchLib.  If not, see <http://www.gnu.org/licenses/>.

 *************************************************************/

/*
 * testtranslation.cpp
 *
 *  Created on: Apr 10, 2011
 *      Author: stolee
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "translation.hpp"

void test( int N, int r, int* set, int* set2 )
{
	int tupleindex = indexOfTuple(N, r, set);
	indexToTuple(N, r, tupleindex, set2);

	bool equal = true;
	for ( int i = 0; i < r; i++ )
	{
		if ( set[i] != set2[i] )
		{
			equal = false;
		}
	}

	if ( !equal )
	{
		printf("-- TUPLE Problem: ");
		for ( int i = 0; i < r; i++ )
		{
			printf("%d ", set[i]);
		}
		printf("-> %d -> ", tupleindex);
		for ( int i = 0; i < r; i++ )
		{
			printf("%d ", set2[i]);
		}
		printf("\n");
	}

	int index = indexOfSet(r, set);
	indexToSet(r, index, set2);

	equal = true;
	for ( int i = 0; i < r; i++ )
	{
		if ( set[i] != set2[i] )
		{
			equal = false;
		}
	}

	if ( !equal )
	{
		printf("-- Problem: ");
		for ( int i = 0; i < r; i++ )
		{
			printf("%d ", set[i]);
		}
		printf("-> %d -> ", index);
		for ( int i = 0; i < r; i++ )
		{
			printf("%d ", set2[i]);
		}
		printf("\n");
	}
}

void testLEX( int N, int r, int* set, int* set2 )
{
	int index = lexIndexOfSet(N, r, set);
	lexIndexToSet(N, r, index, set2);

	bool equal = true;
	for ( int i = 0; i < r; i++ )
	{
		if ( set[i] != set2[i] )
		{
			equal = false;
		}
	}

	if ( !equal )
	{
		printf("-- Problem: ");
		for ( int i = 0; i < r; i++ )
		{
			printf("%d ", set[i]);
		}
		printf("-> %d -> ", index);
		for ( int i = 0; i < r; i++ )
		{
			printf("%d ", set2[i]);
		}
		printf("\n");
	}
}

int main( void )
{
	int r = 4;

	int* set = (int*) malloc(r * sizeof(int));
	int* set2 = (int*) malloc(r * sizeof(int));

	int N = 20;

	initBinomialTable(N, r);

	for ( int a = 0; a < N; a++ )
	{
		if ( r > 1 )
		{
			for ( int b = 0; b < N; b++ )
			{
				if ( b != a )
				{

					if ( r > 2 )
					{
						for ( int c = 0; c < N; c++ )
						{
							if ( c != a && c != b )
							{
								if ( r > 3 )
								{
									for ( int d = 0; d < N; d++ )
									{
										if ( d != a && d != b && d != c )
										{
											set[0] = a;
											set[1] = b;
											set[2] = c;
											set[3] = d;
											test(N, r, set, set2);
										}
									}
								}
								else
								{
									set[0] = a;
									set[1] = b;
									set[2] = c;
									test(N, r, set, set2);
								}
							}
						}
					}
					else
					{
						set[0] = a;
						set[1] = b;

						test(N, r, set, set2);
					}
				}
			}
		}
		else
		{
			set[0] = a;
			test(N, r, set, set2);
		}
	}

	printf("-- TESTING LEX:\n");
	for ( int a = 0; a < N; a++ )
	{
		if ( r > 1 )
		{
			for ( int b = 0; b < N; b++ )
			{
				if ( b != a )
				{

					if ( r > 2 )
					{
						for ( int c = 0; c < N; c++ )
						{
							if ( c != a && c != b )
							{
								if ( r > 3 )
								{
									for ( int d = 0; d < N; d++ )
									{
										if ( d != a && d != b && d != c )
										{
											set[0] = a;
											set[1] = b;
											set[2] = c;
											set[3] = d;
											testLEX(N, r, set, set2);
										}
									}
								}
								else
								{
									set[0] = a;
									set[1] = b;
									set[2] = c;
									testLEX(N, r, set, set2);
								}
							}
						}
					}
					else
					{
						set[0] = a;
						set[1] = b;

						testLEX(N, r, set, set2);
					}
				}
			}
		}
		else
		{
			set[0] = a;
			testLEX(N, r, set, set2);
		}
	}

	int* coverage = (int*) malloc(nChooseK(N, r) * sizeof(int));
	bzero(coverage, nChooseK(N, r) * sizeof(int));

	for ( int i = 0; i < N; i++ )
	{
		for ( int j = 0; j < N; j++ )
		{
			if ( i != j )
			{
				int num_w_ij = numSetsWW(N, r, i, j);
				for ( int sindex = 0; sindex < num_w_ij; sindex++ )
				{
					getSetWW(r, i, j, sindex, set);
					int fullindex = indexOfSet(r, set);

					(coverage[fullindex])++;
				}
			}
		}
	}

	int coverage_val = 2 * nChooseK(r, 2);

	for ( int index = 0; index < nChooseK(N, r); index++ )
	{
		if ( coverage[index] != coverage_val )
		{
			printf("Set index %d was covered WW only %d times instead of %d. ", index, coverage[index], coverage_val);
			indexToSet(r, index, set);
			for ( int i = 0; i < r; i++ )
			{
				printf("%d ", set[i]);
			}
			printf("\n");
		}
	}

	/*** TEST WWO ****/
	bzero(coverage, nChooseK(N, r) * sizeof(int));

	for ( int i = 0; i < N; i++ )
	{
		for ( int j = 0; j < N; j++ )
		{
			if ( i != j )
			{
				int num_w_ij = numSetsWWO(N, r, i, j);
				for ( int sindex = 0; sindex < num_w_ij; sindex++ )
				{
					getSetWWO(r, i, j, sindex, set);
					int fullindex = indexOfSet(r, set);

					(coverage[fullindex])++;
				}
			}
		}
	}

	coverage_val = r * (N - r);

	for ( int index = 0; index < nChooseK(N, r); index++ )
	{
		if ( coverage[index] != coverage_val )
		{
			printf("Set index %d was covered WWO only %d times instead of %d. ", index, coverage[index], coverage_val);
			indexToSet(r, index, set);
			for ( int i = 0; i < r; i++ )
			{
				printf("%d ", set[i]);
			}
			printf("\n");
		}
	}

	/*** TEST WOWO ****/
	bzero(coverage, nChooseK(N, r) * sizeof(int));

	for ( int i = 0; i < N; i++ )
	{
		for ( int j = 0; j < N; j++ )
		{
			if ( i != j )
			{
				int num_w_ij = numSetsWOWO(N, r, i, j);
				for ( int sindex = 0; sindex < num_w_ij; sindex++ )
				{
					getSetWOWO(r, i, j, sindex, set);
					int fullindex = indexOfSet(r, set);

					(coverage[fullindex])++;
				}
			}
		}
	}

	coverage_val = 2 * nChooseK(N - r, 2);

	for ( int index = 0; index < nChooseK(N, r); index++ )
	{
		if ( coverage[index] != coverage_val )
		{
			printf("Set index %d was covered WOWO only %d times instead of %d. ", index, coverage[index], coverage_val);
			indexToSet(r, index, set);
			for ( int i = 0; i < r; i++ )
			{
				printf("%d ", set[i]);
			}
			printf("\n");
		}
	}

	free(set);
	free(set2);

	cleanBinomialTable();
}
