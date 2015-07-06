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
 * pairs.h
 *
 *  Created on: Sep 11, 2009
 *      Author: derrickstolee
 */

#ifndef PAIRS_H_
#define PAIRS_H_


/* unmatched pairs of layer 2 */
int vsToI(int vert1, int vert2);
int poToI(int pair1, int pair2, int orient);
void iToPO(int i, int* pair1, int* pair2, int* orient);
int iToV1(int i);
int iToV2(int i);

/* edges in third layer */
int eToV1(int e);
int eToV2(int e);
int vsToE(int v1, int v2);

/* edges in full graph */
int edgeToV1(int e);
int edgeToV2(int e);
int vsToEdge(int v1, int v2);

/* triples in third layer */
int vsToT(int v1, int v2, int v3);
int tToV1(int t);
int tToV2(int t);
int tToV3(int t);

/* triples in full graph */
int vsToTrip(int v1, int v2, int v3);
int tripToV1(int t);
int tripToV2(int t);
int tripToV3(int t);

/* numerical functions */
int nChooseK(int n, int k);


/**
 * Take the two partitions and join them, modifying p1.
 */
void joinPartitions(int* p1, int* p2, int ne);

/**
 * Given a permutation,
 */
void modifyEdgePartition(int* part, int* perm, int n);




#endif /* PAIRS_H_ */
