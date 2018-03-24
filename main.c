/* Am I in debug mode? */
#define DEBUG 1

#include <stdio.h>
#include <stdlib.h>
#include <time.h> /* for srand(time(NULL)) */
#include <limits.h> /* for MAX_INT */
#include <math.h>
#include <string.h> /* for filename management */
#include <stdint.h> /* for uintptr_t */

#include "grid.h"
#include "image.h"
#include "rules.h"
#include "debug.h"

int main(void) {

    Grid g;
    Grid op;

    g = initGrid(192, 108);
    op = initGrid(7, 7);

    TRACE(("DEBUG MODE ACTIVE.\n"));

    srand(time(NULL));

		initRandomGrid(&g);
    initInverseSquare(&op);
    applyRuleSetMass(&op, 1);
    if (DEBUG) {
      printGrid(&op);
    }

    grid2PNG(&g, "G.png");
    grid2PNG(&op, "OP.png");

    slideshowRuleConvolve(&g, &op, "Convolution.png");

    destroyGrid(g);
    destroyGrid(op);

    return 0;
}
