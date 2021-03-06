#include <stdio.h>
#include <stdlib.h>
#include "grid.h"
#include <string.h>

/*
typedef struct Cell
typedef struct Grid
*/

/* ------- GRID ------- */

Grid initGrid(int width, int height, const char *name) {
	Grid g;

	if (width <= 0 || height <= 0) {
		fprintf(stderr, "ERROR: initGrid: tried to init grid of sizes %d, %d\n", width, height);
		exit(-1);
	}

	g.width = width;
	g.height = height;

	g.curCell = (Cell *) calloc( width * height, sizeof(Cell) );
	g.newCell = (Cell *) calloc( width * height, sizeof(Cell) );

	strcpy(g.name, name);

	return g;
}

void destroyGrid(Grid g) {
    free(g.curCell);
    free(g.newCell);
    return;
}

void commitGridUpdate(Grid *g) {
	Cell *tmp = g->curCell;
	g->curCell = g->newCell;
	g->newCell = tmp;
}

void gridCopy(Grid *source, Grid *destination) {
	int i,j; /* counters */
	if ((source->width != destination->width) || (source->height != destination->height)) {
		fprintf(stderr, "ERRORE: gridCopy: ho cercato copiare una griglia %dx%d in una griglia %dx%d\n", source->width, source->height, destination->width, destination->height );
		exit(-1);
	}

	for (i=0; i<source->width; i++) {
		for (j=0; j<source->height; j++) {
			setCell(destination, i, j, getCell(source, i, j));
		}
	}

	commitGridUpdate(destination);
}

Cell getCell(const Grid *g, int x, int y) {
	if ( x >= g->width || y >= g->height || x<0 || y<0 ) {
		fprintf(stderr, "ERRORE: getCell: ho cercato di accedere alla cella (%d, %d) ma la griglia era di dimensioni (%d, %d)\n", x, y, g->width, g->height );
		exit(-1);
	}
	return g->curCell[ x + y * g->width ];
}

void setCell(Grid *g, int x, int y, Cell cell) {
	if ( x >= g->width || y >= g->height || x<0 || y<0 ) {
		fprintf(stderr, "ERRORE: setCell: ho cercato di accedere alla cella (%d, %d) ma la griglia era di dimensioni (%d, %d)\n", x, y, g->width, g->height );
		exit(-1);
	}
	g->newCell[ x + y * g->width ] = cell;
	return;
}

/* ------- INDEX ------- */

Index At(int x, int y, int nRows, int nCols) {
	Index ind;
	ind.i = x;
	ind.j = y;
	ind.n = nRows;
	ind.m = nCols;
	return ind;
}
