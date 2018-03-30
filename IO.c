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
    printf("got line.\n");
    /*TRACE(("Bufsize: %d\nBuf: %s\n", (int) bufsize, buf));*/
    if ((int) buflen == 2) {
      printf("Is char in string\n");
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
