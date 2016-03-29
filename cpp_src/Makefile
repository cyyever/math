PKG_NAME=my_math
TARGET_LIB=lib$(PKG_NAME).so
HEADERS=$(wildcard *.h)
SRCS=$(wildcard *.cpp)
OBJS=$(patsubst %.cpp,%.o,$(SRCS))
CPPFLAGS=-O2 -Wall -Wextra -fPIC -g -std=c++11
LDFLAGS=-shared 
CXX=clang++
.PHONY:all clean install
all:$(TARGET_LIB)
$(TARGET_LIB):$(OBJS)
	$(CXX) $(LDFLAGS) $^ -o $@
clean:
	-$(RM) $(TARGET_LIB) $(OBJS) *~
install:all
	install -D $(TARGET_LIB) $(DESTDIR)/usr/local/lib/$(PKG_NAME)/$(TARGET_LIB)
	install -d $(DESTDIR)/usr/local/include/$(PKG_NAME)
	install -t $(DESTDIR)/usr/local/include/$(PKG_NAME) $(HEADERS)
	echo "/usr/local/lib/$(PKG_NAME)" > $(DESTDIR)/etc/ld.so.conf.d/$(PKG_NAME).conf
	ldconfig
