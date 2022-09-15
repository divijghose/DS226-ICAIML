CC :=gcc
LDFLAGS := -fopenmp
C_SOURCES :=$(wildcard *.c)
C_EXECUTABLE :=$(C_SOURCES:.c=.out)

all:$(C_EXECUTABLE) 

$(C_EXECUTABLE):$(C_SOURCES)
		$(CC) $(LDFLAGS) $< -o $@

clean:
		rm -rf *.out
