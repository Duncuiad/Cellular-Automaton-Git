/* This takes care of all IO:
** both image file management
** and user interaction via terminal.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/* int mkdir(const char *pathname, mode_t mode); */

#include "IO.h"
#include "debug.h"
#include "grid.h"
#include "init.h"
#include "image.h"
#include "rules.h"

int isCharInString(char c, const char* str);

int getIntFromUser(int min, int max) {
    int value;
    char buf[1024];
    char *end;

    do {
        printf("[%d - %d] ", min, max);

        if (!fgets(buf, sizeof(buf), stdin) ) {
            break;
        }
        /* remuve \n */
        buf[strlen(buf)-1] = 0;

        value = strtol(buf, &end, 10);
    } while( end != buf + strlen(buf) || min > value || value > max);
    /*flushLineFromStdin();*/

    return value;
}

void awaitUserInput(void) {
    char buf[1024];

    fgets(buf, sizeof(buf), stdin);

    return;
}

void flushLineFromStdin() {
    while (getchar() != '\n');
}

char getUserInput(const char* query, const char* allowedAnswers) {
  char * buf;
  size_t bufsize = 32;
  size_t buflen;
  size_t i;

  /* check allowedAnswer and alloc buffer */
  if (strlen(allowedAnswers)<2) {
    fprintf(stderr, "getUserInput: got empty allowedAnswers.\n");
    exit(1);
  }

  buf = (char *)malloc(bufsize * sizeof(char)); /* open buffer */
  if( buf == NULL)
  {
    perror("Unable to allocate buffer");
    exit(1);
  }

  /*  Get input */
  while (1) {

    printf("%s [%c", query, allowedAnswers[0]);
    for (i = 1; i < strlen(allowedAnswers); i++) {
      printf("/%c", allowedAnswers[i] );
    }
    printf("]: ");

    buflen = getline(&buf, &bufsize, stdin);
    TRACE(("got line.\n"));
    /*TRACE(("Bufsize: %d\nBuf: %s\n", (int) bufsize, buf));*/
    if ((int) buflen == 2) {
      TRACE(("Is char in string\n"));
      if (isCharInString(buf[0], allowedAnswers)) {
        return buf[0];
      }
    }
  }

  free(buf); /* close buffer */
}

int isCharInString(char c, const char* str) {
  size_t i;

  for (i = 0; i < strlen(str); i++) {
    if (str[i] == c) {
      return 1;
    }
  }
  return 0;
}


void interactiveMode(Grid grids[MAXGRIDS]) {
    int state = 0;
    int s = 0; /*active grid index*/

    setupGrid(&grids[s]);

    do {
        switch (state) {
            case 0:
            state = mainMenu(grids, &s);
            break;
            case 1:
            state = initMenu(&grids[s]);
            break;
            case 2:
            state = slideshowMenu(grids, &s);
            break;
            case 3:
            state = selectGrid(grids, &s);
            break;
            case 4:
            state = rulesMenu(grids, &s);
            break;
        }
    } while(state >= 0);

    /* cleanup: dealloc all initialized grids? */
}

void printAvailableGrids(Grid *grids) {
    int i;
    printf("Available grids:\n");
    for (i = 0; i < MAXGRIDS; i++) {
        printf("(%d) %s\n", i+1, grids[i].name );
    }
}

void flushScreen(void) {
    int i;
    for ( i = 0; i < 24; i++) {
        printf("\n");
    }
}

void setupGrid(Grid *g) {
    int i, j;
    char name[MAX_NAME_LENGTH];

    printf("New grid. Please name: ");
    fgets(name, MAX_NAME_LENGTH, stdin);
    for (i = 0; i < MAX_NAME_LENGTH-1; i++) {
        if ( isspace(name[i]) ) {
            name[i] = 0;
        }
    }
    name[MAX_NAME_LENGTH-1] = 0;

    printf("Grid width: ");
    i = getIntFromUser(1, MAXSIZE);
    printf("Grid height: ");
    j = getIntFromUser(1, MAXSIZE);

    TRACE(("setupGrid: Calling initGrid(%d, %d, %s)\n", i, j, name));

    *g = initGrid(i, j, name);
}

int selectGrid(Grid *grids, int *s) {
    flushScreen();
    printAvailableGrids(grids);
    printf("Select a grid number: ");
    *s = getIntFromUser(1, 10);
    (*s)--;
    if (strcmp(grids[*s].name, "") == 0) {
        setupGrid(&grids[*s]);
    }
    return 0; /* go to main menu */
}


int mainMenu(Grid *grids, int *s) {
    char c;

    int i, j;
    float curMax, curMin;
    Cell curCell;
    float buf;

    flushScreen();
    TRACE(("DBG: In mainMenu\n"));

    printf("\nCurrent grid: %s\n", grids[*s].name);

    printf("What do you want to do?\n(i) Initialize the current grid\n(r) Apply a rule to the current grid\n(l) Slideshow a rule on the current grid\n(t) Display stats of the current grid\n(s) Select a new grid\n(d) Delete a grid\n(x) Exit\n");
    c = getUserInput("Select one option", "irltsdx");

    switch (c) {
        case 'r':
            return 4;
        case 't':
            printf("Grid: %s\n", grids[*s].name);
            printf("Size: %d x %d\n", grids[*s].width, grids[*s].height);
            /* find min max */
            for ( j = 0; j<grids[*s].height; j++) {
                for ( i = 0; i<grids[*s].width; i++) {
                    curCell = getCell(&grids[*s], i, j);
                    buf = curCell.data;
                    curMin = buf < curMin ? buf : curMin;
                    curMax = buf > curMax ? buf : curMax;
                }
            }
            printf("Range: %f to %f\n", curMin, curMax);
            awaitUserInput();
            return 0; /* go to this menu */
        case 'i':
            return 1; /* go to initMenu */
        case 's':
            return 3;
        case 'l':
            return 2; /* go to slideshowMenu */
        case 'd':
            printAvailableGrids(grids);
            printf("Select a grid to delete: ");
            *s = getIntFromUser(1, 10);
            (*s)--;
            if (strcmp(grids[*s].name, "") != 0) {
                strcpy(grids[*s].name, "");
                destroyGrid(grids[*s]);
            } else {
                printf("Grid slot already empty.\n");
            }
            return 3; /* go to select grid */
        case 'x':
            return -1;
    }
    return 0;
}

int initMenu(Grid *grid) {
    char c;

    char buf[MAX_NAME_LENGTH+4];
    sprintf(buf, "%s.png", grid->name);

    flushScreen();
    TRACE(("DBG: In initMenu\n"));

    printf("Current grid: %s.\n", grid->name);
    printf("How would you like to initialize your grid?\n");
    printf("(a) With random floats\n(b) With random booleans\n(c) with the 2-norm distance from the center\n(x) Back\n");
    c = getUserInput("Select one option: ", "abcx");
    switch (c) {
        case 'a':
            TRACE(("initRandomNoise beginning on grid %s\n", grid->name));
            initRandomNoise(grid);
            TRACE(("initRandomNoise done\n"));
            grid2PNG(grid, buf );
            break;
        case 'b':
            initRandomBool(grid, 0.5);
            grid2PNG(grid, buf );
            break;
        case 'c':
            initInverseSquare(grid);
            grid2PNG(grid, buf );
            break;
        case 'x':
            return 0;
    }

    return 0; /*go to main menu */
}

int slideshowMenu(Grid *grids, int *s) {
    char c;
    char buf[68];
    int op;

    sprintf(buf, "%s.png", grids[*s].name);

    flushScreen();
    TRACE(("DBG: In sludeshowMenu\n"));

    printf("Current grid: %s.\n", grids[*s].name);
    printf("Which rule would you like to slideshow?\n");
    printf("(a) Conway\n(b) Convolution\n(x) Back\n");
    c = getUserInput("Select one option: ", "abx");
    switch (c) {
        case 'a':
            TRACE(("slideshowRuleConway beginning on grid %s\n", buf));
            slideshowRuleConway(&grids[*s], buf);
            TRACE(("initRandomNoise done\n"));
            grid2PNG(&grids[*s], buf );
            break;
        case 'b':
            TRACE(("slideshowRuleConvolve beginning on grid %s\n", buf));
            marker:
            printAvailableGrids(grids);
            printf("Select a grid:");
            op = getIntFromUser(1, 10);
            op--;
            if (strcmp(grids[op].name, "") == 0) {
                printf("Please select a non-empty grid slot.\n");
                goto marker;
            }
            if (*s == op) {
                printf("Cannot convolve a grid with itself.\n");
                return 2; /* go to this menu */
            }
            slideshowRuleConvolve(&grids[*s], &grids[op], buf);
            break;
        case 'x':
            return 0; /* go to main menu */
    }

    return 2; /*go to this menu again */
}

int rulesMenu(Grid *grids, int *s) {
    char c;
    char buf[68];
    int op;

    sprintf(buf, "%s.png", grids[*s].name);

    flushScreen();
    TRACE(("DBG: In rulesMenu\n"));

    printf("Current grid: %s.\n", grids[*s].name);
    printf("Which rule would you like to apply?\n");
    printf("(a) Normalize\n(b) Convolution\n(c) Set Mass\n(x) Back\n");
    c = getUserInput("Select one option: ", "abcx");
    switch (c) {
        case 'a':
            TRACE(("applyRuleNormalize on grid: %s\n", buf));
            applyRuleNormalize(&grids[*s], 0, 1);
            TRACE(("applyRuleNormalize done\n"));
            grid2PNG(&grids[*s], buf );
            break;
        case 'b':
            TRACE(("applyRuleConvolve beginning on grid %s\n", buf));
            marker:
            printAvailableGrids(grids);
            printf("Select a grid:");
            op = getIntFromUser(1, 10);
            op--;
            if (strcmp(grids[op].name, "") == 0) {
                printf("Please select a non-empty grid slot.\n");
                goto marker;
            }
            if (*s == op) {
                printf("Cannot convolve a grid with itself.\n");
                return 4; /* go to this menu */
            }
            applyRuleConvolve(&grids[*s], &grids[op]);
            break;
        case 'c':
            TRACE(("applyRuleSetMass beginning on grid %s", grids[*s].name));
            applyRuleSetMass(&grids[*s], 1);
            TRACE(("applyRuleSetMass done"));
            grid2PNG(&grids[*s], buf );
            break;
        case 'x':
            return 0; /* go to main menu */
    }

    return 4; /*go to this menu again */
}
