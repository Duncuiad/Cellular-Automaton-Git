#include "transform.h"
#include "grid.h"
#include <stdio.h>

/* Options and description listed in the header */
Index transformIndex(Index ind, char option){
    Index temp = ind; /* set temp equal to ind so that the switch-case only affects what actually changes between the two */
    switch (option) {
        case 'i':
            break; /* unchanged indices: notice that temp == ind */
        case 'r':
            temp.i = ind.j;
            temp.j = ind.n - 1 - ind.i;
            temp.n = ind.m;
            temp.m = ind.n;
            break;
        case 'l':
            temp.i = ind.m - 1 - ind.j;
            temp.j = ind.i;
            temp.n = ind.m;
            temp.m = ind.n;
            break;
        case 'c':
            temp.i = ind.n - 1 - ind.i;
            temp.j = ind.m - 1 - ind.j;
            break;
        case 'x':
            temp.j = ind.m - 1 - ind.j;
            break;
        case 'y':
            temp.i = ind.n - 1 - ind.i;
            break;
        case 'd':
            temp.i = ind.j;
            temp.j = ind.i;
            temp.n = ind.m;
            temp.m = ind.n;
            break;
        case 'a':
            temp.i = ind.m - 1 - ind.j;
            temp.j = ind.n - 1 - ind.i;
            temp.n = ind.m;
            temp.m = ind.n;
            break;
        default :
            /* if the chosen option is not valid, it leaves the indices unscathed and prints a default error: */
            fprintf(stderr, "Errore: l'opzione scelta per la trasformazione degli indici non e' valida");
    }
    return temp;
}

/* ------- GRID MANIPULATION -------
// Apply a certain transformation to a grid */

/* Transform the grid */
void transformGrid(Grid *g, char option){
  int i,j; /* counters' standard prior to C99 */
  Index ind = At(0, 0, g->height, g->width);
  Index result = ind; /* set to reduce the length of call to the temp grid coordinates */
  for (i=0; i< g->height; i++) {
      for (j=0; j< g->width; j++) {
          ind.i = i;
          ind.j = j;
          result = transformIndex(ind, option); /* apply transformation to indices */
          g->newCell[ result.j + result.i * result.m ] = getCell(g, j, i); /* set new grid using modified indices (function of the counting indices) */
      }
  }
  commitGridUpdate(g); /* actually update the grid */
  g->width = result.m; /* update grid dimensions */
  g->height = result.n;
}

void complement(Grid *g){
  int i,j;
  Cell tempCell;

  for (i=0; i< g->height; i++) {
    for (j=0; j< g->width; j++) {

      tempCell.data = 1 - getCell(g, i, j).data;
      setCell(g, i, j, tempCell);

    }
  }
  commitGridUpdate(g);
}
