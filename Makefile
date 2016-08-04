BUILDDIR := ./build/
BINARY = $(BUILDDIR)sync

all : dir sync package

ROOTDIR = $(CURDIR)/../
toolchain := $(ROOTDIR)toolchain/
CXX = $(toolchain)/bin/arm-xiaomi-linux-uclibcgnueabi-g++
CC =$(toolchain)/bin/arm-xiaomi-linux-uclibcgnueabi-gcc
LIB_DIR = -L$(ROOTDIR)/lib/
CXXFLAGS += -I$(ROOTDIR)/include/
LDFLAGS = -Wall -O2 -lxmrouter -lthrift -lssl -lcrypto -lconfig++ -ljson-c \
 -lboost_system -lboost_filesystem -lthriftnb -levent -lcurl -lz -lboost_thread \
 -lroutermain

dir : 
	mkdir -p $(BUILDDIR)

sync :
	$(CXX) $(CXXFLAGS) sync.cpp $(LIB_DIR) $(LDFLAGS) -o $(BINARY)   
	
clean:
	rm -r build

.PHONY : clean sync

package: 
	cp start_script build/
	../plugin_packager_x64
