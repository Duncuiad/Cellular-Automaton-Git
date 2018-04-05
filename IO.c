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
