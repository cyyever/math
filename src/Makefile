PKG_NAME=my_math
TARGET_LIB=lib$(PKG_NAME).so
SRCS=$(wildcard *.c)
DEPS=$(patsubst %.c,%.d,$(SRCS))
OBJS=$(patsubst %.c,%.o,$(SRCS))
HEADERS=$(wildcard *.h)
CC=gcc 
CFLAGS=-O2 -Wall -fPIC 
LDFLAGS=-shared 
.PHONY:all clean install
all:
	-make $(TARGET_LIB)
	-$(RM) $(DEPS) $(OBJS) *~
$(TARGET_LIB):$(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@
clean:
	-$(RM) $(TARGET_LIB) $(DEPS) $(OBJS) *~
install:all
	install -D $(TARGET_LIB) $(DESTDIR)/usr/local/lib/$(PKG_NAME)/$(TARGET_LIB)
	install -d $(DESTDIR)/usr/local/include/$(PKG_NAME)
	install -t $(DESTDIR)/usr/local/include/$(PKG_NAME) $(HEADERS)
	echo "/usr/local/lib/$(PKG_NAME)" > $(DESTDIR)/etc/ld.so.conf.d/$(PKG_NAME).conf
	ldconfig
%.d:%.c
	@set -e; $(RM) $@; \
	$(CC) $(CFLAGS) -M $< | sed 's,\($(notdir $*)\)\.o[ :]*,$(dir $*)\1.o $@ : ,g' > $@;
-include $(DEPS)
