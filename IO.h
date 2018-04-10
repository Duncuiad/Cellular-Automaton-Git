#ifndef IO_H_INCLUDED
#define IO_H_INCLUDED

#include "grid.h"

#define MAXSIZE 1000
#define MAXGRIDS 10

void awaitUserInput(void);
char getUserInput(const char* query, const char* allowedAnswers);
int getIntFromUser(int min, int max);
double getFloatFromUser(float min, float max);
double getUnboundedFloatFromUser();
void flushLineFromStdin();

void interactiveMode(Grid grids[MAXGRIDS]);
void setupGrid(Grid *g);
int mainMenu(Grid *grids, int *s);
int initMenu(Grid *grid);
void flushScreen(void);
void printAvailableGrids(Grid *grids);
int selectGrid(Grid *grids, int *s);
int slideshowMenu(Grid *grids, int *s);
int rulesMenu(Grid *grids, int *s);


#endif /*IO_H_INCLUDED*/
