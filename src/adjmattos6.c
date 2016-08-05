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
 * adjmattos6.c
 *
 *  Created on: Nov 18, 2010
 *      Author: derrickstolee
 */

#include <stdio.h>
#include <nauty.h>
#include <nausparse.h>
#include <nautinv.h>
#include <gtools.h>

int main(int argc, char** argv)
{
	if ( argc < 2 )
	{
		printf("Usage: adjmattos6 n\n");
		exit(1);
	}

	int N = atoi(argv[1]);

	char buffer[1500];

	/* buffer holds an s6 string */
	sparsegraph g;
	SG_INIT(g);

	g.nv = N;
	g.v = (size_t*) malloc(N * sizeof(size_t));
	g.d = (int*) malloc(N * sizeof(int));
	g.e = (int*) malloc(N * N * sizeof(int));

	int cur_e = 0;

	int small_n = 0;

	while ( scanf("%s", buffer) != EOF )
	{
		if ( buffer[0] == '0' || buffer[0] == '1' )
		{
			/* Add this vertex */
			g.d[small_n] = 0;
			g.v[small_n] = cur_e;

			int i;
			for ( i = 0; i < N; i++ )
			{
				if ( buffer[i] == '1' )
				{
					g.d[small_n] = g.d[small_n] + 1;
					g.e[cur_e] = i;
					cur_e++;
				}
			}

			small_n++;
		}

		if ( small_n == N )
		{
			g.nde = cur_e;
			g.vlen = N;
			g.dlen = N;
			g.elen = cur_e;
			printf("%s", sgtos6(&g));
			small_n = 0;
			cur_e = 0;
		}
	}

	SG_FREE(g);

	return 0;
}
