/***********************************************************

Copyright Derrick Stolee 2012.

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
 * testcolextranslation.cpp
 *
 *  Created on: Dec 4, 2012
 *      Author: stolee
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "translation.hpp"

int main( int argc, char** argv )
{

	int k = 8;
	int n = 30;

	initBinomialTable(n, k);

	int nchoosek = nChooseK(n, k);

	int* set = (int*) malloc(sizeof(int) * k);
	int* set2 = (int*) malloc(sizeof(int) * k);

	int* markers = (int*) malloc(sizeof(int) * nchoosek);

	for ( int i = 0; i < nchoosek; i++ )
	{
		markers[i] = -1;
	}

	printf("Testing unrank/rank.\n");

	for ( int i = 0; i < nchoosek; i++ )
	{
		/* get set with colex */
		indexToSet(k, i, set);

		int index = indexOfSet(k, set);

		if ( index < 0 || index >= nchoosek )
		{
			printf("SET HAS OUT OF BOUNDS INDEX (%d): ", index);

			for ( int j = 0; j < k; j++ )
			{
				printf("%d ", set[j]);
			}
			printf("\n");
		}
		else
		{
			if ( markers[index] < 0 )
			{
				markers[index] = i;

				indexToSet(k, index, set2);

				bool equal = true;
				for ( int l = 0; l < k; l++ )
				{
					if ( set[l] != set2[l] )
					{
						equal = false;
					}
				}

				if ( !equal )
				{
					printf("UNRANKING DID NOT MATCH AT INDEX(%d): ", index);

					for ( int j = 0; j < k; j++ )
					{
						printf("%d ", set[j]);
					}
					printf("--- and --- ");
					for ( int j = 0; j < k; j++ )
					{
						printf("%d ", set2[j]);
					}
					printf("\n");
				}
			}
			else
			{
				/* CONFLICT! */
				indexToSet(k, markers[index], set2);

				printf("TWO SETS HAVE SAME INDEX (%d): ", index);
				for ( int j = 0; j < k; j++ )
				{
					printf("%d ", set2[j]);
				}
				printf("--- and --- ");
				for ( int j = 0; j < k; j++ )
				{
					printf("%d ", set[j]);
				}
				printf("\n");
			}
		}
	}

	printf("Testing successors.\n");

	indexToSet( k, 0, set);
	int index = 0;

	do
	{
		if ( indexOfSetNoSort(k, set) != index )
		{
			printf("-- DOESN'T MATCH AT %d!\n", index);
		}
		index++;
	}
	while ( getSuccessor( k, set) >= 0 && index < nchoosek );

	printf("Testing predecessors.\n");
	index = nChooseK(n, k) - 1;
	indexToSet(k, index, set);

	do
	{
		if ( indexOfSetNoSort( k, set) != index )
		{
			printf("-- DOESN'T MATCH AT %d!\n", index);
		}
		index--;
	}
	while ( getPredecessor( k, set) >= 0 );

	cleanBinomialTable();
}

