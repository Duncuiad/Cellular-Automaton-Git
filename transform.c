#include "transform.h"
#include "grid.h"
#include <stdio.h>

//Options and description listed in the header
Index transform(Index ind, char option){
    Index temp = ind;
    switch (option) {
        case 'i':
            break;
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
            fprintf(stderr, "Errore: l'opzione scelta per la trasformazione degli indici non e' valida");
    }
    return temp;
}

// Apply a certain transformation to a grid
void transpose(Grid *g){
    Index ind = At(0, 0, g->height, g->width);
    Index result = ind;
    int i,j;
    for (i=0; i< g->height; i++){
        for (j=0; j< g->width; j++) {
            ind.i = i;
            ind.j = j;
            result = transform(ind, 'd');
            setCell(g, result.i, result.j, getCell(g, i, j));
            commitGridUpdate(g);
        }
    }
}
//void reflectHor(Grid *g);
//void reflectVer(Grif *g);
