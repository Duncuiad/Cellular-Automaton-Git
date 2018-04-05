#include "init.h"
#include "debug.h"

#define DEBUG 1

/* sets every cell to random value in [0,1] */
void initRandomNoise(Grid *g){
	int i, j;
	Cell tempCell;

	TRACE(("initRandomNoise on grid size %d, %d", g->width, g->height));

	for (i = 0; i < g->width; i++) {
		for (j = 0; j < g->height; j++) {
			tempCell.data = ((float) rand()/RAND_MAX);
			setCell(g, i, j, tempCell);
		}
	}

	commitGridUpdate(g);
}

/* sets cells to 1 with probability "threshold", to 0 otherwise */
int initRandomBool(Grid *g, float threshold){
  int i, j;
  Cell tempCell;

  if (threshold >= 0 && threshold <= 1){ /* threshold must be a value in [0,1] */
    for (i = 0; i < g->width; i++) {
      for (j = 0; j < g->height; j++) {
        tempCell.data = ((float) rand()/RAND_MAX < threshold) ? 1.f : 0.f; /* checks if the random value is in [0, threshold] and sets cell value accordingly */
        setCell(g, i, j, tempCell);
      }
    }
    commitGridUpdate(g);
		return 1;
  }

  else {
    printf("Hai scelto un valore di probabilità non valido per l'inizializzazione. Scegli un decimale tra 0 e 1\n");
		return 0;
  }

}

/* sets every cell in the grid to value "state" */
void initBlank(Grid *g, float state){
  int i, j;
  Cell tempCell;

  for (i = 0; i < g->width; i++) {
    for (j = 0; j < g->height; j++) {
      tempCell.data = state;
      setCell(g, i, j, tempCell);
    }
  }

  commitGridUpdate(g);
}

void initInverseSquare(Grid *g) {
	int i, j;
	Cell tempCell;
	float centerX = (float) (g->width-1) / 2;
	float centerY = (float) (g->height-1) / 2;
	float temp;

	for ( i = 0; i < g->width; i++) {
		for (j = 0; j < g->height; j++) {
			temp = (centerX-i)*(centerX-i) + (centerY-j)*(centerY-j);
			/* hack to make it work nicely on large grids: */
			temp /= centerX*centerX+centerY*centerY;
			tempCell.data = 1/(temp+1);
			setCell(g, i, j, tempCell);
		}
	}

	commitGridUpdate(g);
}
