#
# libtestgear - Test Gear client library
#

VERSION=0.1alpha

CC=gcc
CFLAGS=-Wall -fPIC -DVERSION=\"$(VERSION)\" -Iinclude -Wno-unused-variable -Wno-unused-function
LDFLAGS=-ltestgear -L.
     
all: libtestgear.so test
     
debug: CFLAGS += -DDEBUG -g
debug: all

objects = testgear.o \
          tcp.o \
          message.o

$(objects): %.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

libtestgear.so: $(objects)
	$(CC) -Wall -shared -Wl,-soname,$@ -o $@ $(objects) -Wl,-init,init

test: libtestgear.so test.c
	$(CC) $(CFLAGS) test.c -o test $(LDFLAGS)

clean:
	rm -f *.o libtestgear.so test

.PHONY: all clean debug
