CXX := clang++
OUTPUTFILE := eSGraph.a

INC=include external/glm

INC_PARAMS=$(foreach d, $(INC), -I$d)
CXXFLAGS := -std=c++11 $(INC_PARAMS) -MMD -MP
LDFLAGS := -shared
LDLIBS :=

SRC_DIR := src
BIN_DIR := bin

srcfiles := $(wildcard $(SRC_DIR)/*.cpp)
objects  := $(patsubst %.cpp, %.o, $(srcfiles))

all: $(OUTPUTFILE)

$(OUTPUTFILE): $(objects)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(OUTPUTFILE) $(objects) $(LDLIBS)

depend: .depend

.depend: $(srcfiles)
	rm -f ./.depend
	$(CXX) $(CXXFLAGS) -MM $^>>./.depend;

clean:
	rm -f $(objects)
	rm -f $(OUTPUTFILE)

dist-clean: clean
	rm -f *~ .depend

include .depend