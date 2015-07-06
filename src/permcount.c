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
 * permcount.c
 *
 *  Created on: Jul 19, 2010
 *      Author: derrickstolee
 *
 *  Take a list of sparse6 representations, 
 * 		take complements, and count the number
 * 		of perfect matchings.
 *  
 */

#include <stdio.h>
#include <string.h>
#include <nauty.h>
#include <nausparse.h>
#include <nautinv.h>
#include <gtools.h>

int getConjC(int p);
int countPM(sparsegraph* g, int max_p);

int ERR_UPDATE = 10000000;
int MIN_P = 2;
int MAX_P = 1000;

int main(int argc, char** argv)
{
	int goal_min_p = 1;
	int goal_max_p = 10;

	if ( argc >= 2 )
	{
		goal_min_p = atoi(argv[1]);
		goal_max_p = goal_min_p;
	}

	if ( argc >= 3 )
	{
		goal_max_p = atoi(argv[2]);
	}

	char buffer[500];
	long long int count = 0;

	while ( scanf("%s", buffer) != EOF )
	{
		if ( strlen(buffer) == 0 )
		{
			continue;
		}

		count++;

		if ( count % ERR_UPDATE == 0 )
		{
			fprintf(stderr, "> COUNT: %010lld\n", count);
		}

		/* buffer holds an s6 string */
		sparsegraph g;
		SG_INIT(g);

		int num_loops;
		stringtosparsegraph(buffer, &g, &num_loops);
		int c = (g.nde / 2) - ((g.nv * g.nv) / 4);

		int p = countPM(&g, goal_max_p);

		if ( goal_min_p <= p && p <= goal_max_p )
		{
			printf("%5d\t%3d\t%s\n", p, c, buffer);
		}

		SG_FREE(g);
	}

	return 0;
}

int getConjC(int p)
{
	int t, dblfact, k;
	int last_dblfact;

	t = 1;
	last_dblfact = 1;
	dblfact = 1;

	while ( dblfact <= p )
	{
		t++;
		last_dblfact = dblfact;
		dblfact *= (2 * t - 1);
	}

	k = p / last_dblfact;

	return (t - 1) * (t - 1) - (t - 1) + k - 1;
}

int countPM(sparsegraph* g, int max_p)
{
	/* find first vertex non-negative */
	int i;
	int vert = -1;
	for ( i = 0; i < g->nv; i++ )
	{
		if ( g->v[i] >= 0 )
		{
			vert = i;
			break;
		}
	}

	if ( vert < 0 )
	{
		/* oh noes! NO undone vertices???? */
		/* this is actually a good thing! */
		/* we found a perfect matching */
		return 1;
	}

	int vert_pos = g->v[vert];
	int count = 0;

	for ( i = vert_pos; i < vert_pos + g->d[vert]; i++ )
	{
		int vert2 = g->e[i];

		if ( g->v[vert2] >= 0 )
		{
			/* unmatched! */
			int vert2_pos = g->v[vert2];

			/* count with this edge */
			g->v[vert] = g->v[vert2] = -1;

			count += countPM(g, max_p);

			g->v[vert] = vert_pos;
			g->v[vert2] = vert2_pos;

			if ( count > max_p )
			{
				/* too many already! */
				return count;
			}
		}
	}

	return count;
}

