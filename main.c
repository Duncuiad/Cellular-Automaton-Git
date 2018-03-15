#include <stdio.h>
#include <stdlib.h>
#include <time.h> // for srand(time(NULL))
#include <string.h> // for filename management
#include "lodepng.h"
#include <stdint.h> // for uintptr_t
#include "grid.h"
#include "image.h"

// Esempio: faccio la media di tutte le celle in un intorno quadrato di raggio dato
int ruleX(Grid g, int x, int y, int radius) {
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

void applyRuleX(Grid *g) {
	Cell curCell;
	for ( int x = 0; x < g->width; x++) {
		for ( int y = 0; y < g->height; y++) {
			curCell.data = ruleX( *g, x, y, 1);
			setCell(g, x, y, curCell);
		}
	}
	commitGridUpdate(g);
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
    Grid g = initGrid(10, 10);

    Cell temp;
    temp.data = 17;

    setCell(&g, 2, 2, temp);

    for (int j = 0; j < g.height; j++) {
        for (int i = 0; i < g.width; i++) {
            temp.data = (i+j)*(i+j);
            setCell(&g, i, j, temp);
        }
    }
    commitGridUpdate(&g);

    printGrid(g);
    grid2PNG(g, "img1.png");

    applyRuleX(&g);

    printf("\nI have applied rule X to the above grid.\n\n");

    printGrid(g);
    grid2PNG(g, "img3.png");

    destroyGrid(g);


    return 0;
}
