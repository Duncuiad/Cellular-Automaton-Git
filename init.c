#include "init.h"

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

void initRandomBool(Grid *g, float threshold){
  int i, j;
  Cell tempCell;

  if (threshold >= 0 && threshold <= 1){
    for (i = 0; i < g->width; i++) {
      for (j = 0; j < g->height; j++) {
        tempCell.data = ((float) rand()/RAND_MAX < threshold) ? 1.f : 0.f;
        setCell(g, i, j, tempCell);
      }
    }
    commitGridUpdate(g);
  }

  else {
    printf("Hai scelto un valore di probabilità non valido per l'inizializzazione. Scegli un decimale tra 0 e 1");
  }

}

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
