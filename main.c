#include <stdio.h>
#include <stdlib.h>
#include <time.h> // for srand(time(NULL))
#include <string.h> // for filename management
#include "lodepng.h"
#include <stdint.h> // for uintptr_t
#include "grid.h"
#include "image.h"

// Esempio: faccio la media di tutte le celle in un intorno quadrato di raggio dato
int ruleAverage(Grid g, int x, int y, int radius) {
	int hitCount = 0; // celle valide
	int runningTotal = 0;
	Cell curCell;
	// Questo e' facilmente ottimizzabile
	for ( int i = -1*radius; i <= radius; i++) {
		for ( int j = -1*radius; j <= radius; j++) {
			if (0 <= x+i && x+i < g.width && 0 <= y+j && y+j < g.height ) {
				hitCount++;
				curCell = getCell(g, x+i, y+j);
				runningTotal += curCell.data;
			}
		}
	}
	return runningTotal / hitCount;
}

int ruleConvolve(Grid tgt, Grid op, int x, int y) {
    int runningCount = 0;
    int centerX = op.width/2;
    int centerY = op.height/2;
    Cell curCellTgt;
    Cell curCellOp;
    int opTot = 0;

    for ( int j = 0; j < op.height; j++) {
        for ( int i = 0; i < op.width; i++) {
            curCellTgt = getCell(tgt, (x-centerX+i)%tgt.width , (y-centerY+j)%tgt.height );
            curCellOp = getCell(op, i, j);
            opTot += curCellOp.data;
            runningCount += curCellOp.data * curCellTgt.data;
        }
    }

    if (opTot!=0) return runningCount/opTot;
    else return runningCount;
}

void applyRuleConvolve(Grid *tgt, Grid op) {
    Cell curCell;
	for ( int x = 0; x < tgt->width; x++) {
		for ( int y = 0; y < tgt->height; y++) {
			curCell.data = ruleConvolve( *tgt, op, x, y);
			setCell(tgt, x, y, curCell);
		}
	}
	commitGridUpdate(tgt);
}

void applyRuleAverage(Grid *g) {
	Cell curCell;
	for ( int x = 0; x < g->width; x++) {
		for ( int y = 0; y < g->height; y++) {
			curCell.data = ruleAverage( *g, x, y, 5);
			setCell(g, x, y, curCell);
		}
	}
	commitGridUpdate(g);
}

void slideshowRuleConvolve(Grid *g, Grid op, const char *filename) {
    char buffer[64];
    do {
        printf("Looping...\n");
        applyRuleConvolve(g, op);
        grid2PNG(*g, filename);
    } while (strcmp(fgets(buffer, 64, stdin), "exit\n") != 0);
    return;
}

// debug
void printGrid(Grid g) {
    // printf("Printing grid:\n");
    for (int j = 0; j < g.height; j++) {
        for (int i = 0; i < g.width; i++) {
            Cell c = getCell(g, i, j);
            printf("|%4u ", c.data);
        }
        printf("|\n\n");
    }
}

int main()
{
    Grid g = initGrid(50, 50);

    Grid op = initGrid(5, 5);

    Cell temp;

    srand(time(NULL));

    for (int j = 0; j < g.height; j++) {
        for (int i = 0; i < g.width; i++) {
            temp.data = rand()%9999;
            setCell(&g, i, j, temp);
        }
    }
    commitGridUpdate(&g);

    for (int j = 0; j < op.height; j++) {
        for (int i = 0; i < op.width; i++) {
            temp.data = i+j-3;
            setCell(&op, i, j, temp);
        }
    }
    commitGridUpdate(&op);
/*
    printGrid(g);
    grid2PNG(g, "img1.png");

    printf(" ----------------------- \n\n");

    applyRuleConvolve(&g, op);
    printGrid(g);
    grid2PNG(g, "img2.png");

    printf(" ----------------------- \n\n");

    applyRuleConvolve(&g, op);
    printGrid(g);
    grid2PNG(g, "img3.png");
*/
    slideshowRuleConvolve(&g, op, "slideshow.png");


    destroyGrid(g);
    destroyGrid(op);

    return 0;
}
