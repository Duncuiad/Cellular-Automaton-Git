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

int main()
{
    Grid g = initGrid(192, 108);

    Grid op = initGrid(7, 7);

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


/*
    // Initialize op with the x, y position of each cell
    double centerX = (op.width-1) / 2;
    double centerY = (op.height-1) / 2;
    for (int j = 0; j < op.height; j++) {
        for (int i = 0; i < op.width; i++) {
            cell.data = i+j;
            setCell(&op, i, j, cell);
        }
    }
    commitGridUpdate(&op);
*/

/*
    // Initialize op with a bottom-right-corner unbalanced distribution
    double centerX = (op.width-1) / 2;
    double centerY = (op.height-1) / 2;
    for (int j = 0; j < op.height; j++) {
        for (int i = 0; i < op.width; i++) {
            cell.data = 0;
            if (i > centerX && j > centerY) cell.data ++;
            setCell(&op, i, j, cell);
        }
    }
    commitGridUpdate(&op);
*/

    // Make op have a total value of 1
    printGrid(op);
    printf(" ----\n\n");
    applyRuleSetMass(&op, 1);
    printGrid(op);
    grid2PNG(op, "OP.png");

    // Convolve
    slideshowRuleConvolve(&g, op, "slideshow.png");

    grid2PNG(g, "G.png");


    destroyGrid(g);
    // destroyGrid(op);

    return 0;
}
