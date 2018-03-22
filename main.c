#include <stdio.h>
#include <stdlib.h>
#include <time.h> // for srand(time(NULL))
#include <limits.h> // for MAX_INT
#include <math.h>
#include <string.h> // for filename management
#include <stdint.h> // for uintptr_t

#include "lodepng.h"

#include "grid.h"
#include "image.h"
#include "rules.h"


// debug
void printGrid(Grid g) {
    // printf("Printing grid:\n");
    for (int j = 0; j < g.height; j++) {
        for (int i = 0; i < g.width; i++) {
            Cell c = getCell(g, i, j);
            printf("| %5f ", c.data);
        }
        printf("|\n\n");
    }
}

int main()
{
    Grid g = initGrid(192, 108);


    Cell cell;
    cell.data = 1;

    srand(time(NULL));

    // Initialize g with random doubles in [0, 1)
    double tmp;
    for (int j = 0; j < g.height; j++) {
        for (int i = 0; i < g.width; i++) {

            tmp = rand();
            tmp = tmp / RAND_MAX;

            cell.data = tmp;
            setCell(&g, i, j, cell);
        }
    }
    commitGridUpdate(&g);

		initRuleConway(&g);
		printGrid(g);
		slideshowRuleConway(&g, "slideshow.png");


/*
    Grid op = initGrid(7, 7);
    // Initialize op with the inverse squared distance to center
    double centerX = (op.width-1) / 2; //[Federico] Non so se è intenzionale, ma qui e alla riga sotto, la divisione utilizzata è la divisione intera
    double centerY = (op.height-1) / 2;	//[Federico] Dovrebbe bastare dividere per 2.0
    for (int j = 0; j < op.height; j++) {
        for (int i = 0; i < op.width; i++) {
            double myDist = fabs(i-centerX)+fabs(j-centerY)+1;
            cell.data = pow( (1/myDist), 2);
            setCell(&op, i, j, cell);
        }
    }
    commitGridUpdate(&op);

    // Make op have a total value of 1
    printGrid(op);
    printf(" ----\n\n");
    applyRuleSetMass(&op, 1);
    printGrid(op);
    grid2PNG(op, "OP.png");

    // Convolve
    slideshowRuleConvolve(&g, op, "slideshow.png");
		destroyGrid(op);

*/
    grid2PNG(g, "G.png");


    destroyGrid(g);

    return 0;
}
