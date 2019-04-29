#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>


#define MAXLINE 64
#define MAXFILENAME 64


double GetTime(void);
int GetRefval(char*, double*);
int PathConv(char*, char *);
int GetFilename(const char*, char (*Filies)[MAXFILENAME]);
int ExFilename(char*, char*, char*, char*);
int CopyLogFile(FILE*);

