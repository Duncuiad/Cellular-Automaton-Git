#ifndef _init_h
#define _init_h

#include <stdio.h>
#include <stdlib.h>
#include "grid.h"

void initRandomNoise(Grid *g);
void initRandomBool(Grid *g, float treshold);
void initBlank(Grid *g, float state);

#endif
