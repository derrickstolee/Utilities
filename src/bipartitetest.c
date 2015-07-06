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
 * bipartitetest.c
 *
 *  Created on: Jul 20, 2010
 *      Author: derrickstolee
 *
 * Read in a list of bipartite graphs in g6 or s6 format.
 * Count the number of perfect matchings.
 */

#include <stdio.h>
#include <string.h>
#include <nauty.h>
#include <nausparse.h>
#include <nautinv.h>
#include <gtools.h>


int getConjC(int p);
int countPM(sparsegraph* g);
int countPMpart(sparsegraph* g, char* vert_list, int v);


int countBipartiteLPM(int k, sparsegraph* g);
int countBipartiteLPMpart(int k, sparsegraph* g, char* left_list, int v);

int countBipartiteRPM(int k, sparsegraph* g);
int countBipartiteRPMpart(int k, sparsegraph* g, char* left_list, int v);



int ERR_UPDATE = 10000;
int MIN_P = 1;
int MAX_P = 10000;


int main(int argc, char** argv)
{
	
	char buffer[500];
	long long int count = 0;
	int balanced_count = 0;

	while ( scanf("%s", buffer) != EOF )
	{
		if ( strlen(buffer) == 0 )
		{
			continue;
		}
		
		count++;
		
		if ( count % ERR_UPDATE == 0 )
		{
			fprintf(stderr, "> COUNT: %lld\n", count);
		}
		
		/* buffer holds an s6 string */
		sparsegraph g;
		SG_INIT(g);

		int num_loops;
		stringtosparsegraph(buffer, &g, &num_loops);
		
		int k = g.nv / 2;
		
		/* check balanced bipartiteness */
		int i, j;
		int found_bad_edge = 0;
		for ( i = 0; i < k; i++ )
		{
			for ( j = 0; j < g.d[i]; j++ )
			{
				if ( g.e[g.v[i]+j] < k )
				{
					found_bad_edge = 1;
					break;	
				}	
			}
			
			if ( found_bad_edge == 0 )
			{
				for ( j = 0; j < g.d[i+k]; j++ )
				{
					if ( g.e[g.v[i+k]+j] >= k )
					{
						found_bad_edge = 1;
						break;	
					}	
				}
			}
			
			if ( found_bad_edge == 1 )
			{
				break;
			}
		}
		
		if ( found_bad_edge == 0 )
		{
			balanced_count++;
			int p = countBipartiteLPM(k, &g);
			int c = (g.nde / 2 + (k * (k - 1)) / 2) - ((g.nv * g.nv) / 4);
		
			if (  (p > 1 && c >= getConjC(p) ) )
			{
				printf("%5d\t%3d\t%s (L)\n", p, c, buffer);
			}	
		}
	
		SG_FREE(g);
	}

	printf("BALANCED COUNT: %d\n", balanced_count);
		
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
	
	return (t-1)*(t-1) - (t-1) + k - 1;
}




int countPM(sparsegraph* g)
{
	char* vert_list = (char*)malloc(g->nv);
	
	int i;
	for ( i = 0; i < g->nv; i++ )
	{
		vert_list[i] = 0;
	}
	
	int count = countPMpart(g, vert_list, 0);
	
	free(vert_list);
	
	return count;
}



int countPMpart(sparsegraph* g, char* vert_list, int v)
{	
	while ( v < g->nv && vert_list[v] != 0 )
	{
		v++;
	}
	
	if ( v >= g->nv )
	{			
		return 1;
	}
	
	vert_list[v] = 1;
	
	int i;	
	int count = 0;
	for ( i = 0; i < g->d[v]; i++ )
	{
		int u = g->e[g->v[v] + i];
		
		if ( v < u && vert_list[u] == 0 )
		{
			vert_list[u] = 1;
		
			int sub_count = countPMpart(g, vert_list, v + 1);
			
			count = count + sub_count;
			
			vert_list[u] = 0;
		}
	}
	
	vert_list[v] = 0;
	
	return count;
}





int countBipartiteLPM(int k, sparsegraph* g)
{
	char* left_list = (char*)malloc(k);
	bzero(left_list, k);
	
	int count = countBipartiteLPMpart(k, g, left_list, 0);
	
	free(left_list);
	
	return count;
}



int countBipartiteLPMpart(int k, sparsegraph* g, char* left_list, int v)
{	
	if ( v > k )
	{
		return 1;	
	}
	
	int count = 0;
	
	int i;
	for ( i = 0; i < g->d[v]; i++ )
	{
		int u =  g->e[g->v[v]+i] - k;
		
		if ( left_list[u] == 0 )
		{
			/* mark it! */
			left_list[u] = 1;
			
			count += countBipartiteLPMpart(k, g, left_list, v+1);
			
			/* unmark it! */
			left_list[u] = 0;
			
			if ( count > MAX_P )
			{
				return count;	
			}
		}	
	} 
	
	return count;
}

	



int countBipartiteRPM(int k, sparsegraph* g)
{
	char* right_list = (char*)malloc(k);
	bzero(right_list, k);
	
	int count = countBipartiteRPMpart(k, g, right_list, 0);
	
	free(right_list);
	
	return count;
}



int countBipartiteRPMpart(int k, sparsegraph* g, char* right_list, int v)
{	
	if ( v > k )
	{
		return 1;	
	}
	
	int count = 0;
	
	int i;
	for ( i = 0; i < g->d[v+k]; i++ )
	{
		int u = g->e[g->v[v+k]+i];
		if ( right_list[u] == 0 )
		{
			/* mark it! */
			right_list[u] = 1;
			
			count += countBipartiteRPMpart(k, g, right_list, v+1);
			
			/* unmark it! */
			right_list[u] = 0;
			
			if ( count > MAX_P )
			{
				return count;	
			}
		}	
	} 
	
	return count;
}

	
