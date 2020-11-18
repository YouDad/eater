export
CC := g++
CPPFLAGS := -g -O3
CPPFLAGS += -std=c++11
CPPFLAGS += -I$(CURDIR)/include/
LDFLAGS := -lpthread

OUTDIR := $(CURDIR)/out
objs := $(patsubst %.cpp, %.o, $(wildcard $(CURDIR)/src/*/*.cpp))

.PHONY: default
default: src clean

.PHONY: src
src:
    [ -e $(OUTDIR) ] || mkdir -p $(OUTDIR)
    make -C src/
    make install -C src/

.PHONY: test
test: src
    make clean
    make -C test/
    rm -rf $(OUTDIR)
    mkdir -p $(OUTDIR)
    make install -C test/
    make clean

.PHONY: clean
clean:
    -find -name "*.o" | xargs rm -f
