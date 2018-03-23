#include <stdio.h>
#include <stdlib.h>
#include <time.h> /* for srand(time(NULL)) */
#include <limits.h> /* for MAX_INT */
#include <math.h>
#include <string.h> /* for filename management */
#include <stdint.h> /* for uintptr_t */

#include "lodepng.h"

#include "grid.h"
#include "image.h"
#include "rules.h"

int main(void) {

    /* Initialize g with random doubles in [0, 1) */
/*    double tmp;
    int j = 0;
    int i = 0;
    Cell cell; */
    Grid g;


    /* cell.data = 1; */
    g = initGrid(192, 108);

    srand(time(NULL));

		initInverseSquare(&g);
		printGrid(&g);

    grid2PNG(&g, "G.png");


    destroyGrid(g);

    return 0;
}
