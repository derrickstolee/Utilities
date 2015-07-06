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
 * translation.hpp
 *
 *  Created on: Apr 10, 2011
 *      Author: stolee
 */

#ifndef TRANSLATION_HPP_
#define TRANSLATION_HPP_

/**********************/

/* Now, nChooseK is calculated with a table, when initialized */
void initBinomialTable(int maxN, int maxK);
void cleanBinomialTable();

/**
 * nChooseK
 */
int nChooseK(int n, int k);

/**
 * indexOf
 *
 * The co-lex index of the pair (i,j).
 */
int indexOf(int i, int j);

/**
 * indexToPair
 */
void indexToPair(int index, int& i, int& j);

/**
 * indexOfOrderedPair(i,j)
 *
 * When order matters!
 */
int indexOfOrderedPair(int i, int j);

/**
 * indexToOrderedPair(index, i, j)
 *
 * When order matters!
 */
void indexToOrderedPair(int index, int& i, int& j);

/**
 * indexOfSet
 *
 * Get the co-lex order of the set of a given size.
 */
int indexOfSet(int size, int* set);

/**
 * indexOfSet
 *
 * Get the co-lex order of the set of a given size.
 */
int indexOfSetNoSort(int size, int* set);

/**
 * indexToSet
 */
void indexToSet(int size, int index, int* set);

/**
 * getSuccessor
 *
 * Increment in colex-order in-place.
 *
 * @return the index for increment
 */
int getSuccessor(int size, int* set);

/**
 * getLexPredeccessor
 *
 * Decrement in colex-order in-place.
 *
 * @return the index for dencrement
 */
int getPredecessor(int size, int* set);

/**
 * lexIndexOfSet
 *
 * Get the lex order of the set of a given size.
 */
int lexIndexOfSet(int n, int size, int* set);

/**
 * lexIndexToSet
 */
void lexIndexToSet(int n, int size, int index, int* set);

/**
 * getLexSuccessor
 *
 * Increment in lex-order in-place.
 *
 * @return the index for increment
 */
int getLexSuccessor(int n, int size, int* set);

/**
 * getLexPredeccessor
 *
 * Decrement in lex-order in-place.
 *
 * @return the index for dencrement
 */
int getLexPredecessor(int n, int size, int* set);

/**
 * sortSet
 */
void sortSet(int size, int* set);

/**
 * indexOfTuple
 *
 * Get the product order of the set of a given size.
 */
int indexOfTuple(int n, int size, int* tuple);

/**
 * indexToTuple
 */
void indexToTuple(int n, int size, int index, int* tuple);

/**
 * numSetsW
 *
 * Get the number of sets with i
 */
int numSetsW(int n, int s, int i);

/**
 * numSetsWW
 *
 * Get the number of sets of size s
 * 	within [n] with BOTH entries
 */
int numSetsWW(int n, int s, int i, int j);

/**
 * numSetsWWO
 *
 * Get the number of sets with first entry, but NOT second.
 */
int numSetsWWO(int n, int s, int i, int j);

/**
 * numSetsWOWO
 *
 * Get the number of sets without either entry.
 */
int numSetsWOWO(int n, int s, int i, int j);

/**
 * getSetW
 */
void getSetW(int s, int i, int index, int* set);

/**
 * getSetWW
 */
void getSetWW(int s, int i, int j, int index, int* set);

/**
 * getSetWWO
 */
void getSetWWO(int s, int i, int j, int index, int* set);

/**
 * getSetWOWO
 */
void getSetWOWO(int s, int i, int j, int index, int* set);

#endif /* TRANSLATION_HPP_ */
