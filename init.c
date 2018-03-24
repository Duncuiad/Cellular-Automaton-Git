#include "init.h"

/* sets every cell to random value in [0,1] */
void initRandomNoise(Grid *g){
	int i, j;
	Cell tempCell;

	for (i = 0; i < g->width; i++) {
		for (j = 0; j < g->height; j++) {
			tempCell.data = ((float) rand()/RAND_MAX);
			setCell(g, i, j, tempCell);
		}
	}

	commitGridUpdate(g);
}

/* sets cells to 1 with probability "threshold", to 0 otherwise */
void initRandomBool(Grid *g, float threshold){
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
  }

  else {
    printf("Hai scelto un valore di probabilità non valido per l'inizializzazione. Scegli un decimale tra 0 e 1");
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
