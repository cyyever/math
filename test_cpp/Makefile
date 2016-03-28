SRCS=$(wildcard *.cpp)
BINS=$(patsubst %.cpp,%,$(SRCS))
CPPFLAGS=-O2 -Wall -g -fPIC -I/usr/local/include/my_math -std=c++11
LDFLAGS=-L/usr/local/lib/my_math -lmy_math
.PHONY:all clean

all:$(BINS)
clean:
	-$(RM) $(BINS) *.o *~ 
