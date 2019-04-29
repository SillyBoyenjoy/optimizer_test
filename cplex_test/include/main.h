/*************************************************************************
	> File Name: main.h
	> Author: Liu Chong
	> Mail: 
	> Created Time: 2019年03月18日 星期一 12时12分17秒
 ************************************************************************/
#include <ilcplex/cplex.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include <auxfunc.h>

#define MAXFINENAME 64
#define MAXFILIES   256
#define LP   ".lp"
#define MPS  ".mps"  
#define SOL  ".sol"

#define READ_CASE_FAIL 1
#define CPLEX_OPT_FAIL 2
#define CASE_INFESIABLE 4
#define TIME_OUT       8
#define SOL_FAULT      16
#define ACCEPT         32

typedef struct{
    short prob;
    short test_fes;
    short test_sit;
} TESTenv;

static void free_and_null(char **);
static void usage(char *);









