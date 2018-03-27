#ifndef _init_h
#define _init_h

#include <stdio.h>
#include <stdlib.h>
#include "grid.h"

void initRandomNoise(Grid *g); /* sets every cell to random value in [0,1] */
int initRandomBool(Grid *g, float treshold); /* sets cells to 1 with probability "threshold", to 0 otherwise */
void initBlank(Grid *g, float state); /* sets every cell in the grid to value "state" */

void initInverseSquare(Grid *g);

#endif
