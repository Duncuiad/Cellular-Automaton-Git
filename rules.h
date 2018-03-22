#ifndef _rules_h
#define _rules_h

#include <stdio.h>
#include <stdlib.h>
#include <time.h> // for srand(time(NULL))
#include <limits.h> // for MAX_INT
#include <math.h>
#include <string.h> // for filename management
#include "lodepng.h"
#include <stdint.h> // for uintptr_t
#include "grid.h"
#include "image.h"

Cell ruleAverage(Grid g, int x, int y, int radius);
void applyRuleAverage(Grid *g);

Cell ruleConvolve(Grid tgt, Grid op, int x, int y);
void applyRuleConvolve(Grid *tgt, Grid op);

Cell ruleConway(Grid tgt, int x, int y);
void applyRuleConway(Grid *g);

void applyRuleNormalize(Grid *g, double tgtMin, double tgtMax);

void slideshowRuleConway(Grid *g, const char *filename);

//debug
void printGrid(Grid g);

#endif
