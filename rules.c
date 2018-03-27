#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "rules.h"
#include "grid.h"
#include "image.h"
#include "debug.h"

/*
  ORGANIZZAZIONE DI QUESTA COSA
  Nel file sono elencate le regole, in ordine alfabetico di nome:
  // NOME

  Per ogni regola sono elencate, quando presenti:
  - L'inizializzatore initRuleNome
  - L'iteratore cella per cella ruleNome
  - L'operatore sulla griglia applyRuleNome
  - Il presentatore slideshowRuleNome

*/

/* AVERAGE
// Esempio: faccio la media di tutte le celle in un intorno quadrato di raggio dato
*/
Cell ruleAverage(const Grid *g, int x, int y, int radius) {
	int hitCount = 0; /* celle valide */
	float runningTotal = 0;
	Cell curCell;
	int i;
	int j;
	/* Questo e' facilmente ottimizzabile */
	for ( i = -1*radius; i <= radius; i++) {
		for ( j = -1*radius; j <= radius; j++) {
			if (0 <= x+i && x+i < g->width && 0 <= y+j && y+j < g->height ) {
				hitCount++;
				curCell = getCell(g, x+i, y+j);
				runningTotal += curCell.data;
			}
		}
	}
	curCell.data = runningTotal / hitCount;
	return curCell;
}

void applyRuleAverage(Grid *g) {
  Cell curCell;
	int x;
	int y;

  for ( x = 0; x < g->width; x++) {
    for ( y = 0; y < g->height; y++) {
      curCell = ruleAverage( g, x, y, 5);
      setCell(g, x, y, curCell);
    }
  }
  commitGridUpdate(g);
}

/* CONVOLVE */
Cell ruleConvolve(const Grid *tgt, const Grid *op, int x, int y) {
    float runningCount = 0;
    int centerX = op->width/2;
    int centerY = op->height/2;
    Cell curCellTgt;
    Cell curCellOp;
		int j;
		int i;

    for ( j = 0; j < op->height; j++) {
        for ( i = 0; i < op->width; i++) {
            curCellTgt = getCell(tgt, (x-centerX+i+tgt->width)%tgt->width , (y-centerY+j+tgt->height)%tgt->height );
            curCellOp = getCell(op, i, j);
            runningCount += curCellOp.data * curCellTgt.data;
        }
    }

    curCellTgt.data = runningCount;
    return curCellTgt;
}

void applyRuleConvolve(Grid *tgt, const Grid *op) {
  Cell curCell;
	int x;
	int y;
	int cpuTime;
	clock_t timer = clock();

  for ( x = 0; x < tgt->width; x++) {
    for ( y = 0; y < tgt->height; y++) {
      curCell = ruleConvolve( tgt, op, x, y);
      setCell(tgt, x, y, curCell);
    }
  }
  commitGridUpdate(tgt);
	timer = clock() - timer;
	cpuTime = timer * 1000 / CLOCKS_PER_SEC;
	printf("Convolution took %d.%03d sec.\n", cpuTime/1000, cpuTime%1000);
}

void slideshowRuleConvolve(Grid *g, const Grid *op, const char *filename) {
  char buffer[64];
  do {
    applyRuleConvolve(g, op);
    grid2PNG(g, filename);
		printf("Type \"exit\" to quit the slideshow.\n");
  } while (strcmp(fgets(buffer, 64, stdin), "exit\n") != 0);
  return;
}

/* CONWAY */
Cell ruleConway(const Grid *tgt, int x, int y) {
	int hitCount = 0;
	Cell curCell;
	int i;
	int j;

	/* Questo e' facilmente ottimizzabile */
	for ( i = -1; i <= 1; i++) {
		for ( j = -1; j <= 1; j++) {
            curCell = getCell(tgt, (x+i+tgt->width)%tgt->width, (y+j+tgt->height)%tgt->height );
            if (curCell.data == 1) hitCount++;
        }
	}

	if (hitCount == 3) {
        curCell.data = 1;
        return curCell;
	} else if (hitCount == 4) {
        curCell = getCell(tgt, x, y);
        return curCell;
	}
	curCell.data = 0;
	return curCell;
}

void applyRuleConway(Grid *g) {
	Cell curCell;
	int x;
	int y;

	TRACE(("Applying Rule Conway.\n"));

	for ( x = 0; x < g->width; x++) {
		for ( y = 0; y < g->height; y++) {
			/* printf("Rule Conway: setting cell (%d, %d).\n", x, y); */
			curCell = ruleConway( g, x, y);
			setCell(g, x, y, curCell);
		}
	}
	commitGridUpdate(g);
}

void slideshowRuleConway(Grid *g, const char *filename) {
  char buffer[64];
  do {
    applyRuleConway(g);
    grid2PNG(g, filename);
		printf("Type \"exit\" to quit the slideshow.\n");
  } while (strcmp(fgets(buffer, 64, stdin), "exit\n") != 0);
  return;
}

/* NORMALIZE */
void applyRuleNormalize(Grid *g, double tgtMin, double tgtMax) {
    Cell curCell = getCell(g, 0, 0);
    double curMin = curCell.data;
    double curMax = curCell.data;
    double buf;
		int j;
		int i;
		double distortion;
		double data;

    /* Find current minimum and maximum */
    for ( j = 0; j<g->height; j++) {
        for ( i = 0; i<g->width; i++) {
            curCell = getCell(g, i, j);
            buf = curCell.data;
            curMin = buf < curMin ? buf : curMin;
            curMax = buf > curMax ? buf : curMax;
        }
    }
    /*printf("Normalize: curMin = %f; curMax = %f\n", curMin, curMax);*/

    /* Now, actually normalize */
    distortion = (tgtMax-tgtMin)/(curMax-curMin);
    for ( j = 0; j<g->height; j++) {
        for ( i = 0; i<g->width; i++) {
            curCell = getCell(g, i, j);
            data = curCell.data;
            data = ((data-curMin)*distortion)+tgtMin;
            curCell.data = data;
            setCell(g, i, j, curCell);
        }
    }

    commitGridUpdate(g);
}

/* SETMASS */
void applyRuleSetMass(Grid *g, double tgtMass) {
    Cell curCell;
    double curMass = 0;
		int j;
		int i;
		double distortion;
    /* Find current mass */
    for ( j = 0; j < g->height; j++) {
        for ( i = 0; i < g->width; i++) {
            curCell = getCell(g, i, j);
            curMass += curCell.data;
            /* printf("SetMass: curMass = %f\n", curMass); */
        }
    }
    printf("SetMass: Done: curMass = %f\n", curMass);

    /* Now, actually normalize */
		distortion = tgtMass/curMass;
    for ( j = 0; j<g->height; j++) {
        for ( i = 0; i<g->width; i++) {
            curCell = getCell(g, i, j);
            curCell.data = curCell.data * distortion;
            setCell(g, i, j, curCell);
        }
    }

    commitGridUpdate(g);
}

/* UTILITIES */
void printGrid(const Grid *g) {
		int i;
		int j;

		printf("Printing grid:\n");
    for ( j = 0; j < g->height; j++) {
        for ( i = 0; i < g->width; i++) {
            Cell c = getCell(g, i, j);
            printf("| %5f ", c.data);
        }
        printf("|\n\n");
    }
}
