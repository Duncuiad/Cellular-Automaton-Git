#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rules.h"
#include "grid.h"
#include "image.h"

// Esempio: faccio la media di tutte le celle in un intorno quadrato di raggio dato
Cell ruleAverage(Grid g, int x, int y, int radius) {
	int hitCount = 0; // celle valide
	float runningTotal = 0;
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
	curCell.data = runningTotal / hitCount;
	return curCell;
}


Cell ruleConvolve(Grid tgt, Grid op, int x, int y) {
    float runningCount = 0;
    int centerX = op.width/2;
    int centerY = op.height/2;
    Cell curCellTgt;
    Cell curCellOp;

    for ( int j = 0; j < op.height; j++) {
        for ( int i = 0; i < op.width; i++) {
            curCellTgt = getCell(tgt, (x-centerX+i)%tgt.width , (y-centerY+j)%tgt.height );
            curCellOp = getCell(op, i, j);
            runningCount += curCellOp.data * curCellTgt.data;
        }
    }

    curCellTgt.data = runningCount;
    return curCellTgt;
}

Cell ruleConway(Grid tgt, int x, int y) {
	int hitCount = 0; // celle valide
	Cell curCell;
	// Questo e' facilmente ottimizzabile
	for ( int i = -1; i <= 1; i++) {
		for ( int j = -1; j <= 1; j++) {
            curCell = getCell(tgt, (x+i)%tgt.width, (y+j)%tgt.height );
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
	for ( int x = 0; x < g->width; x++) {
		for ( int y = 0; y < g->height; y++) {
			curCell = ruleConway( *g, x, y);
			setCell(g, x, y, curCell);
		}
	}
	commitGridUpdate(g);
}

void applyRuleConvolve(Grid *tgt, Grid op) {
    Cell curCell;
	for ( int x = 0; x < tgt->width; x++) {
		for ( int y = 0; y < tgt->height; y++) {
			curCell = ruleConvolve( *tgt, op, x, y);
			setCell(tgt, x, y, curCell);
		}
	}
	commitGridUpdate(tgt);
}

void applyRuleAverage(Grid *g) {
	Cell curCell;
	for ( int x = 0; x < g->width; x++) {
		for ( int y = 0; y < g->height; y++) {
			curCell = ruleAverage( *g, x, y, 5);
			setCell(g, x, y, curCell);
		}
	}
	commitGridUpdate(g);
}

void applyRuleNormalize(Grid *g, double tgtMin, double tgtMax) {
    Cell curCell = getCell(*g, 0, 0);
    double curMin = curCell.data;
    double curMax = curCell.data;
    double buf;
    // Find current minimum and maximum
    for (int j = 0; j<g->height; j++) {
        for (int i = 0; i<g->width; i++) {
            curCell = getCell(*g, i, j);
            buf = curCell.data;
            curMin = buf < curMin ? buf : curMin;
            curMax = buf > curMax ? buf : curMax;
        }
    }
    //printf("Normalize: curMin = %f; curMax = %f\n", curMin, curMax);

    // Now, actually normalize
    double distortion = (tgtMax-tgtMin)/(curMax-curMin);
    double data;
    for (int j = 0; j<g->height; j++) {
        for (int i = 0; i<g->width; i++) {
            curCell = getCell(*g, i, j);
            data = curCell.data;
            data = ((data-curMin)*distortion)+tgtMin;
            curCell.data = data;
            setCell(g, i, j, curCell);
        }
    }

    commitGridUpdate(g);
}


void applyRuleSetMass(Grid *g, double tgtMass) {
    Cell curCell;
    double curMass = 0;
    // Find current mass
    for (int j = 0; j < g->height; j++) {
        for (int i = 0; i < g->width; i++) {
            curCell = getCell(*g, i, j);
            curMass += curCell.data;
            // printf("SetMass: curMass = %f\n", curMass);
        }
    }
    printf("SetMass: Done: curMass = %f\n", curMass);

    // Now, actually normalize
    double distortion = tgtMass/curMass;
    for (int j = 0; j<g->height; j++) {
        for (int i = 0; i<g->width; i++) {
            curCell = getCell(*g, i, j);
            curCell.data = curCell.data * distortion;
            setCell(g, i, j, curCell);
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

void slideshowRuleConway(Grid *g, const char *filename) {
    char buffer[64];
    do {
        printf("Looping...\n");
        grid2PNG(*g, filename);
        applyRuleConway(g);
    } while (strcmp(fgets(buffer, 64, stdin), "exit\n") != 0);
    return;
}
