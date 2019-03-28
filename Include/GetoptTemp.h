#ifndef GETOPT_H_
#define GETOPT_H_

void GetoptInit();
char Getopt(int argc, char (*argv)[MAX_COMMAND_LENGTH], const char *shortopts);

extern int optInd;

#endif
