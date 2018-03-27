/* This takes care of all IO:
** both image file management
** and user interaction via terminal.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/* int mkdir(const char *pathname, mode_t mode); */

#include "IO.h"
#include "debug.h"

int isCharInString(char c, const char* str);

char getUserInput(const char* query, const char* allowedAnswers) {
  char * buf;
  size_t bufsize;
  size_t i;

  if (strlen(allowedAnswers)<2) {
    fprintf(stderr, "getUserInput: got empty allowedAnswers.\n");
    exit(1);
  }

  while (1) {
      /*   SEGFAULT?!?!?!?! */
    printf("%s [%c", query, allowedAnswers[0]);
    for (i = 1; i < strlen(allowedAnswers); i++) {
      printf("/%c", allowedAnswers[i] );
    }
    printf("]: ");

    getline(&buf, &bufsize, stdin);
    printf("got line.\n");
    /*TRACE(("Bufsize: %d\nBuf: %s\n", (int) bufsize, buf));*/
    if (strlen(buf) == 2) {
      printf("Is char in string\n");
      if (isCharInString(buf[0], allowedAnswers)) {
        return buf[0];
      }
    }
    free(buf);
  }
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
