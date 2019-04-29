GRBDIR      = .

CC  = gcc -O0

COPT  = -m64 -g

GRBLIBDIR   = $(GRBDIR)/lib

# For dynamic linking
GRBBINDIR 	= $(GRBDIR)/bin
GRBOBJDIR 	= $(GRBDIR)/obj
GRBLIB    	= gurobi81

CLNDIRS   = -L$(GRBBINDIR)
CLNFLAGS  = -l$(GRBLIB) -lm -lpthread

GRBINCDIR   = $(GRBDIR)/include

SRCC          = ./src

CFLAGS  = $(COPT)  -I$(GRBINCDIR)
SETS    = BenchmarkSets/
ARGVS   = $(SETS) \
		  -Threads=4 \
		  -Gap=0.01 \
		  -Screen=on \
		  -Log=on


# ------------------------------------------------------------

$(GRBBINDIR)/gurobi_test: $(GRBOBJDIR)/main.o $(GRBOBJDIR)/auxfunc.o
	$(CC) $(CFLAGS) -o $(GRBBINDIR)/gurobi_test $(GRBOBJDIR)/main.o $(GRBOBJDIR)/auxfunc.o $(CLNDIRS) $(CLNFLAGS)  
$(GRBOBJDIR)/main.o: $(SRCC)/main.c
	$(CC) -c $(CFLAGS) $(SRCC)/main.c -o $(GRBOBJDIR)/main.o -g
$(GRBOBJDIR)/auxfunc.o: $(SRCC)/auxfunc.c
	$(CC) -c $(CFLAGS) $(SRCC)/auxfunc.c -o $(GRBOBJDIR)/auxfunc.o -g

run:
	make
	$(GRBBINDIR)/gurobi_test $(ARGVS)

# Local Variables:
# mode: makefile
# End:
