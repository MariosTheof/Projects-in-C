#ifndef MYSH_H_
#define MYSH_H_

void readLine(void);

void process(void);

int findSpecialSymbol(char **args);

void extractCommand(char* symbol);

void inRedirection(void);

void piping(void);

#endif
