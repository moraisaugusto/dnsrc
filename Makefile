OUT	= dnsRC

VERSION = 0.1

PREFIX    ?= /usr/local

BUILD	= release
TARGET = dnsRC

# build configurations
CXX = g++
CXXFLAGS=-g `pkg-config --cflags --libs giomm-2.4`

# default target
all:
	$(CXX) $(CXXFLAGS) main.cpp -o dnsRC

install: all
	install -m755 dnsRC "$(DESTDIR)$(PREFIX)/bin/dnsRC"

uninstall:
	$(RM) "$(DESTDIR)$(PREFIX)/bin/dnsRC"

clean:
	$(RM) $(OUT) $(OBJ)

.PHONY: clean dist doc install uninstall

