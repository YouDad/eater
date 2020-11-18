main_objs := $(patsubst %.cpp, %.o, $(wildcard *.cpp))
.PHONY: client
client: $(objs)
    $(CC) $(CPPFLAGS) -o client main.cpp $(objs) $(LDFLAGS)
    $(CC) $(CPPFLAGS) -o analyzer analyzer.cpp $(objs) $(LDFLAGS)

.PHONY: install
install:
    mv client $(OUTDIR)/
    mv analyzer $(OUTDIR)/

.PHONY: clean
clean:
    -rm *.o
