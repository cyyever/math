CC=cc
SRCS=$(wildcard *.c)
BINS=$(patsubst %.c,%,$(SRCS))
CFLAGS=-O2 -Wall -g  -fPIC -I/usr/local/include/my_math
LDFLAGS=-L/usr/local/lib/my_math -lmy_math
.PHONY:all clean

all:
	-make $(BINS)
	-$(RM) $(OBJS)
%:%.c
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)
clean:
	-$(RM) $(PROGS) *.o *~ 
