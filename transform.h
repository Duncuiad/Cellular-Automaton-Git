#ifndef _transform_h
#define _transform_h

#include "grid.h"

/*Transforms the input index with respect to the choosen rule */
Index transformIndex(Index ind, char option);
/* Options:
'i': keep unchanged
'r': rotate clocwise 90 degrees
'l': rotate counterclockwise 90 degrees
'c': rotate 180 degrees
'x': reflect horizontally
'y': reflect vertically
'd': reflect with respect to the diagonal (transpose)
'a': reflect with respect to the antidiagonal
*/

/* Apply a certain transformation to a grid */
void transformGrid(Grid *g, char option);

#endif
