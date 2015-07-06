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
 * pairs.c
 *
 *  Created on: Sep 11, 2009
 *      Author: derrickstolee
 */

#include <stdio.h>
#include <stdlib.h>
#include "pairs.hpp"

extern int CONST_V; /* number of verts */
extern int CONST_D; /* degree */
extern int CONST_L; /* how many adjacent verts share */
extern int CONST_T; /* how many non-adjacent verts share */

int vsToI(int vert1, int vert2)
{
	if ( vert1 > vert2 )
	{
		return vsToI(vert2, vert1);
	}
	else if ( vert1 == vert2 - 1 && 0 == (vert1 % 2) )
	{
		return -1;
	}

	int i;
	int index = 0;
	for ( i = 0; i < vert1; i++ )
	{
		index += CONST_D - 2 - 2 * (i / 2);
	}

	index += (vert2 - vert1) - (1 - (vert1 % 2)) - 1;

	return index;
}

int poToI(int pair1, int pair2, int orient)
{
	/* compute the count of this pair */
	int j;
	int pairs = CONST_D / 2;
	int pcount = 0;
	for ( j = 0; j < pair1; j++ )
	{
		pcount += pairs - j - 1;
	}

	pcount += pair2 - pair1 - 1;

	return pcount * 4 + orient;
}

void iToPO(int i, int* pair1, int* pair2, int* orient)
{
	*orient = i % 4;
	int pair_count = i / 4;

	int pairs = CONST_D / 2;
	int j;
	for ( j = 1; j < pairs; j++ )
	{
		if ( pair_count >= pairs - j )
		{
			pair_count -= pairs - j;
		}
		else
		{
			(*pair1) = j - 1;
			(*pair2) = pair_count + (*pair1) + 1;
			return;
		}
	}
}

int iToV1(int i)
{
	int pair1, pair2, orient;
	iToPO(i, &pair1, &pair2, &orient);

	if ( (2 & orient) == 0 )
	{
		return 2 * pair1;
	}
	else
	{
		return 2 * pair1 + 1;
	}
}

int iToV2(int i)
{
	int pair1, pair2, orient;
	iToPO(i, &pair1, &pair2, &orient);

	if ( (1 & orient) == 0 )
	{
		return 2 * pair2;
	}
	else
	{
		return 2 * pair2 + 1;
	}
}

/**
 * Take the two partitions and join them, modifying p1.
 */
void joinPartitions(int* p1, int* p2, int ne)
{
	int i;
	int changed = 1;

	while ( changed > 0 )
	{
		changed = 0;

		for ( i = 0; i < ne; i++ )
		{
			if ( p1[i] < i && p2[i] < i )
			{
				if ( p1[p1[i]] < p1[p2[i]] )
				{
					/* the first partition has a lower value */
					if ( p1[i] > p1[p1[i]] )
					{
						p1[i] = p1[p1[i]];
						changed = 1;
					}
				}
				else
				{
					/* the second partition has a lower value */
					if ( p1[i] > p1[p2[i]] )
					{
						p1[i] = p1[p2[i]];
						changed = 1;
					}
				}
			}
			else if ( p2[i] < i )
			{
				if ( p1[i] > p1[p2[i]] )
				{
					p1[i] = p1[p2[i]];
					changed = 1;
				}
			}
			else if ( p1[i] < i )
			{
				if ( p1[i] > p1[p1[i]] )
				{
					p1[i] = p1[p1[i]];
					changed = 1;
				}
			}
		}
	}
}

/**
 * Given a permutation, create the pair orbits for the layer 2 vertices.
 */
void modifyEdgePartition(int* part, int* perm, int n)
{
	int i;

	int start_i, start_j, start;
	int num_pairs = CONST_V - CONST_D - 1; /* n is even! */
	int* part2 = (int*) malloc(num_pairs * sizeof(int));

	for ( start = 0; start < num_pairs; start++ )
	{
		part2[start] = num_pairs + 20;
	}

	for ( start = 0; start < num_pairs; start++ )
	{
		if ( part2[start] > start )
		{
			/* has not been modified yet! */
			part2[start] = start;

			start_i = iToV1(start);
			start_j = iToV2(start);
			int perm_count = 1;

			int p_i = perm[start_i];
			int p_j = perm[start_j];
			int p_s = vsToI(p_i, p_j);

			while ( p_s != start )
			{
				if ( p_s >= 0 && p_s < num_pairs )
				{
					/* mark */
					part2[p_s] = start;
				}

				/* move! */
				p_i = perm[p_i];
				p_j = perm[p_j];
				p_s = vsToI(p_i, p_j);
				perm_count++;
			}
		}
	}

	joinPartitions(part, part2, num_pairs);

	free(part2);
}

/* edges/pairs */
int eToV1(int e)
{
	int i;

	int n = CONST_V - (CONST_D + 1); /* 84 */
	int eprime = e;
	for ( i = 0; i < n; i++ )
	{
		if ( eprime >= n - 1 - i )
		{
			eprime -= n - 1 - i;
		}
		else
		{
			return i;
		}
	}

	return i;
}

int eToV2(int e)
{
	int i;
	int n = CONST_V - (CONST_D + 1); /* 84 */
	int eprime = e;
	for ( i = 0; i < n; i++ )
	{
		if ( eprime >= n - 1 - i )
		{
			eprime -= n - 1 - i;
		}
		else
		{
			return eprime + i + 1;
		}
	}

	return i + 1;
}

int vsToE(int v1, int v2)
{
	if ( v1 > v2 )
	{
		return vsToE(v2, v1);
	}

	/* sum of i=1,...,CONST_V-1 minus sum of i=1,...,CONST_V - 1 - v1 */
	int n = CONST_V - (CONST_D + 1);
	int ep = (n - 1) * (n) - (n - 1 - v1) * (n - v1);
	return ep / 2 + (v2 - v1 - 1);
}

/* edges/pairs */
int edgeToV1(int e)
{
	int i;

	int n = CONST_V; /* 99 */
	int eprime = e;
	for ( i = 0; i < n; i++ )
	{
		if ( eprime >= n - 1 - i )
		{
			eprime -= n - 1 - i;
		}
		else
		{
			return i;
		}
	}

	return i;
}

int edgeToV2(int e)
{
	int i;
	int n = CONST_V; /* 99 */
	int eprime = e;
	for ( i = 0; i < n; i++ )
	{
		if ( eprime >= n - 1 - i )
		{
			eprime -= n - 1 - i;
		}
		else
		{
			return eprime + i + 1;
		}
	}

	return i + 1;
}

int vsToEdge(int v1, int v2)
{
	if ( v1 > v2 )
	{
		return vsToEdge(v2, v1);
	}

	/* sum of i=1,...,CONST_V-1 minus sum of i=1,...,CONST_V - 1 - v1 */
	int n = CONST_V; /* 99 */
	int ep = (n - 1) * (n) - (n - 1 - v1) * (n - v1);
	return ep / 2 + (v2 - v1 - 1);
}



int vsToT(int v1, int v2, int v3)
{
	if ( v1 > v2 )
	{
		return vsToT(v2, v1, v3);
	}

	int n = CONST_V - (CONST_D + 1); /* 84 */

	/* start with number of pairs (i,j) times number of preceding k */
	int t = v3 * (n - 1) * (n - 2) / 2;

	/* make parameters relative to [N] \ {v3} */
	if ( v1 > v3 )
	{
		v1--;
	}
	if ( v2 > v3 )
	{
		v2--;
	}

	int ep = (n - 2) * (n - 1) - (n - 2 - v1) * (n - 1 - v1);
	int pair_val = ep / 2 + (v2 - v1 - 1);

	t += pair_val;

	return t;
}

int tToV1(int t)
{
	int n = CONST_V - (CONST_D + 1); /* 84 */

	int v3 = (t * 2) / ((n - 1) * (n - 2));

	int remainder = t - (v3 * (n - 1) * (n - 2) / 2);

	int i;
	int v1;
	for ( i = 0; i < n - 1; i++ )
	{
		if ( remainder >= n - 2 - i )
		{
			remainder -= n - 2 - i;
		}
		else
		{
			v1 = i;
			break;
		}
	}

	if ( v1 >= v3 )
	{
		v1++;
	}

	return v1;
}

int tToV2(int t)
{
	int n = CONST_V - (CONST_D + 1); /* 84 */

	int v3 = (t * 2) / ((n - 1) * (n - 2));

	int remainder = t - v3 * (n - 1) * (n - 2) / 2;

	int i;
	int v1;
	for ( i = 0; i < n - 1; i++ )
	{
		if ( remainder >= n - 2 - i )
		{
			remainder -= n - 2 - i;
		}
		else
		{
			v1 = i;
			break;
		}
	}

	if ( v1 >= v3 )
	{
		v1++;
	}

	int v2 = remainder + v1 + 1;

	if ( v1 < v3 && v2 >= v3 )
	{
		v2++;
	}

	return v2;
}

int tToV3(int t)
{
	int n = CONST_V - (CONST_D + 1); /* 84 */

	int v3 = (t * 2) / ((n - 1) * (n - 2));

	return v3;
}

int vsToTrip(int v1, int v2, int v3)
{
	if ( v1 > v2 )
	{
		return vsToTrip(v2, v1, v3);
	}

	int n = CONST_V; /* 99 */

	/* start with number of pairs (i,j) times number of preceding k */
	int t = v3 * (n - 1) * (n - 2) / 2;

	/* make parameters relative to [N] \ {v3} */
	if ( v1 > v3 )
	{
		v1--;
	}
	if ( v2 > v3 )
	{
		v2--;
	}

	int ep = (n - 2) * (n - 1) - (n - 2 - v1) * (n - 1 - v1);
	int pair_val = ep / 2 + (v2 - v1 - 1);

	t += pair_val;

	return t;
}

int tripToV1(int t)
{
	int n = CONST_V; /* 99 */

	int v3 = tripToV3(t);

	int remainder = t - (v3 * (n - 1) * (n - 2) / 2);

	int i;
	int v1;
	for ( i = 0; i < n - 1; i++ )
	{
		if ( remainder >= n - 2 - i )
		{
			remainder -= n - 2 - i;
		}
		else
		{
			v1 = i;
			break;
		}
	}

	if ( v1 >= v3 )
	{
		v1++;
	}

	return v1;
}

int tripToV2(int t)
{
	int n = CONST_V; /* 99 */

	int v3 = tripToV3(t);

	int remainder = t - v3 * (n - 1) * (n - 2) / 2;

	int i;
	int v1;
	for ( i = 0; i < n - 1; i++ )
	{
		if ( remainder >= n - 2 - i )
		{
			remainder -= n - 2 - i;
		}
		else
		{
			v1 = i;
			break;
		}
	}

	if ( v1 >= v3 )
	{
		v1++;
	}

	int v2 = remainder + v1 + 1;

	if ( v1 < v3 && v2 >= v3 )
	{
		v2++;
	}

	return v2;
}

int tripToV3(int t)
{
	int n = CONST_V; /* 99 */

	int v3 = t / (((n - 1) * (n - 2)) / 2);

	return v3;
}


int nChooseK(int n, int k)
{
	int val = 1;
	int i;

	for ( i = n; i > n - k; i-- )
	{
		val *= i;
	}

	for ( i = 2; i <= k; i++ )
	{
		val /= i;
	}

	return val;
}
