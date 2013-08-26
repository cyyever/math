CC=gcc
CFLAGS=-O2 -Wall -g -fPIC -I/usr/local/include/my_math
LDFLAGS=-lmy_math -L/usr/local/lib/my_math
PROGS=my_rat_test my_arithmetic_test
.PHONY:all clean
all:$(PROGS)
clean:
	-$(RM) $(PROGS) *.o *~ 
