IDIR =../include
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=obj
LDIR =../lib

LIBS=-lm

LDFLAGS := -fopenmp

_DEPS = vectHandling.h vectOps.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = assignment2.out daxpy.out dotProduct.out l2Norm.out sumArray.out vectHandling.out matvectmult.out
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.out: %.c $(DEPS)
	$(CC) -c $(LDFLAGS) -o $@ $< $(CFLAGS)

../assignment2.out: $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.out *~ core $(INCDIR)/*~