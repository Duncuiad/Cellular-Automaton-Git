#ifndef IO_H_INCLUDED
#define IO_H_INCLUDED

char getUserInput(const char* query, const char* allowedAnswers);

int getIntFromUser(int min, int max);

void flushLineFromStdin();


#endif /*IO_H_INCLUDED*/
