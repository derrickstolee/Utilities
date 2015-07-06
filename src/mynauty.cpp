/*
 * mynauty.c
 *
 *  Created on: Oct 13, 2009
 *      Author: derrickstolee
 */


#include "mynauty.h"
#include "nausparse.h"


#define BIAS6 63
#define MAXBYTE 126
#define SMALLN 62
#define SMALLISHN 258047
#define TOPBIT6 32
#define C6MASK 63

int s6len = 0;

DYNALLSTAT(char,gcode,gcode_sz);  /* Used by ntog6, ntos6 and sgtos6 */


/****************************************************************************/

static void
encodegraphsize(int n, char **pp)
/* Encode the size n in a string starting at **p, and reset **p
   to point to the character after the size */
{
    char *p;

    p = *pp;
    if (n <= SMALLN)
        *p++ = BIAS6 + n;
    else if (n <= SMALLISHN)
    {
        *p++ = MAXBYTE;
        *p++ = BIAS6 + (n >> 12);
        *p++ = BIAS6 + ((n >> 6) & C6MASK);
        *p++ = BIAS6 + (n & C6MASK);
    }
    else
    {
        *p++ = MAXBYTE;
        *p++ = MAXBYTE;
        *p++ = BIAS6 + (n >> 30);
        *p++ = BIAS6 + ((n >> 24) & C6MASK);
        *p++ = BIAS6 + ((n >> 18) & C6MASK);
        *p++ = BIAS6 + ((n >> 12) & C6MASK);
        *p++ = BIAS6 + ((n >> 6) & C6MASK);
        *p++ = BIAS6 + (n & C6MASK);
    }

    *pp = p;
}

/****************************************************************************/

char* my_sgtos6(sparsegraph* sg)
/* Convert undirected sparse graph to sparse6 string including '\n'.
  It is null-terminated and its address (static memory) is returned.
  The length, not including the null, is put in s6len. */
{
    int *v,*d,*e;
    int i,j,n;
    char *p,x,*plim;
    int nb,topbit;
    int vj,dj,k,lastj;
    int r,rr,l;
    size_t ii;

    SG_VDE(sg,v,d,e);
    n = sg->nv;
    for (i = n-1, nb = 0; i != 0 ; i >>= 1, ++nb) {}

    ii = (nb+1)*(n/6+sg->nde/3);
    DYNALLOC1(char,gcode,gcode_sz,ii+1000,"ntos6");
    plim = gcode + gcode_sz - 20;

    gcode[0] = ':';
    p = gcode;
    *p++ = ':';
    encodegraphsize(n,&p);

    topbit = 1 << (nb-1);
    k = 6;
    x = 0;

    lastj = 0;
    for (j = 0; j < n; ++j)
    {
    vj = v[j];
    dj = d[j];
    for (l = 0; l < dj; ++l)
    {
        i = e[vj+l];
        if (i <= j)
        {
            if (p >= plim)
            {
                ii = p - gcode;
                DYNREALLOC(char,gcode,gcode_sz,
                        5*(gcode_sz/4)+1000,"sgtos6");
                p = gcode + ii;
                plim = gcode + gcode_sz - 20;
            }
            if (j == lastj)
            {
                x <<= 1;
                if (--k == 0)
                {
                    *p++ = BIAS6 + x;
                    k = 6;
                    x = 0;
                }
            }
            else
            {
                x = (x << 1) | 1;
                if (--k == 0)
                {
                    *p++ = BIAS6 + x;
                    k = 6;
                    x = 0;
                }
                if (j > lastj+1)
                {
                    for (r = 0, rr = j; r < nb; ++r, rr <<= 1)
                    {
                        if (rr & topbit) x = (x << 1) | 1;
                        else             x <<= 1;
                        if (--k == 0)
                        {
                            *p++ = BIAS6 + x;
                            k = 6;
                            x = 0;
                        }
                    }
                    x <<= 1;
                    if (--k == 0)
                    {
                        *p++ = BIAS6 + x;
                        k = 6;
                        x = 0;
                    }
                }
                lastj = j;
            }
            for (r = 0, rr = i; r < nb; ++r, rr <<= 1)
            {
                if (rr & topbit) x = (x << 1) | 1;
                else             x <<= 1;
                if (--k == 0)
                {
                    *p++ = BIAS6 + x;
                    k = 6;
                    x = 0;
                }
            }
        }
    }
    }

    if (k != 6)
    {
        if (k >= nb+1 && lastj == n-2 && n == (1<<nb))
	    *p++ = BIAS6 + ((x << k) | ((1 << (k-1)) - 1));
        else
	    *p++ = BIAS6 + ((x << k) | ((1 << k) - 1));
    }

    *p++ = '\n';
    *p = '\0';
    s6len = p - gcode;
    return gcode;
}
