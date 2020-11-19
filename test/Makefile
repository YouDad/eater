test_objs := $(patsubst %.cpp, %.o, $(shell find * -name "*.cpp"))

.PHONY: default
default: $(test_objs) $(objs)
	for object in $(shell find * -name "*.cpp" | sed 's/\.cpp//;'); do \
		filename=`echo $${object} | sed 's/\//./g;'`; \
		$(CC) -o $${filename}.test $${object}.o $(objs) $(LDFLAGS); \
	done;

.PHONY: install
install:
	mv *.test $(OUTDIR)/
