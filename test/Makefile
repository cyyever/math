CC=gcc
CFLAGS=-O2 -Wall -g -fPIC -I/usr/local/include/my_math
LDFLAGS=-L/usr/local/lib/my_math -lmy_math
PROGS=my_rat_test my_arithmetic_test
.PHONY:all clean

all:$(PROGS)
%:%.c
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)
clean:
	-$(RM) $(PROGS) *.o *~ 
