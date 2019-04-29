CPLEXDIR      = .

CC  = gcc -O0

COPT  = -m64 -fPIC -fno-strict-aliasing

# CPLEXJARDIR   = $(CPLEXDIR)/lib/cplex.jar
CPLEXLIBDIR   = $(CPLEXDIR)/lib
CONCERTLIBDIR = $(CPLEXDIR)/lib

# For dynamic linking
CPLEXBINDIR   = $(CPLEXDIR)/bin
CPLEXOBJDIR   = $(CPLEXDIR)/obj
CPLEXLIB      = cplex$(dynamic:yes=1290)
# run           = $(dynamic:yes=LD_LIBRARY_PATH=$(CPLEXBINDIR))
CLNDIRS   = -L$(CPLEXLIBDIR) $(dynamic:yes=-L$(CPLEXBINDIR))
CLNFLAGS  = -l$(CPLEXLIB) -lm -lpthread -ldl

CPLEXINCDIR   = $(CPLEXDIR)/include

EXDIR         = $(CPLEXDIR)/examples
EXINC         = $(EXDIR)/include
SRCC          = ./src

CFLAGS  = $(COPT)  -I$(CPLEXINCDIR)
SETS    = BenchmarkSets/
ARGVS   = $(SETS) \
		  -Threads=4 \
		  -Gap=0.01 \
		  -Screen=off \
		  -Log=on


# ------------------------------------------------------------

cplex_test: $(CPLEXOBJDIR)/main.o $(CPLEXOBJDIR)/auxfunc.o
	$(CC) $(CFLAGS) $(CLNDIRS) -o $(CPLEXBINDIR)/cplex_test -g $(CPLEXOBJDIR)/main.o $(CPLEXOBJDIR)/auxfunc.o $(CLNFLAGS)
$(CPLEXOBJDIR)/main.o: $(SRCC)/main.c
	$(CC) -c $(CFLAGS) $(SRCC)/main.c -o $(CPLEXOBJDIR)/main.o -g
$(CPLEXOBJDIR)/auxfunc.o: $(SRCC)/auxfunc.c
	$(CC) -c $(CFLAGS) $(SRCC)/auxfunc.c -o $(CPLEXOBJDIR)/auxfunc.o -g

run: $(SRCC)/main.c $(SRCC)/auxfunc.c
	make
	$(CC) -c $(CFLAGS) $(SRCC)/main.c -o $(CPLEXOBJDIR)/main.o -g
	$(CPLEXBINDIR)/cplex_test $(ARGVS)
	rm clone*

# Local Variables:
# mode: makefile
# End:
