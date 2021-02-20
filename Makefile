CXX := clang++
OUTPUTFILE := eSGraph.a

INC=include external/glm

INC_PARAMS=$(foreach d, $(INC), -I$d)
CXXFLAGS := -std=c++11 -fPIC $(INC_PARAMS)
LDFLAGS := -shared
LDLIBS :=

SRC_DIR := src
BIN_DIR := bin

srcfiles := $(wildcard $(SRC_DIR)/*.cpp)
objects  := $(patsubst %.cpp, %.o, $(srcfiles))

all: $(OUTPUTFILE)

$(OUTPUTFILE): $(objects)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(OUTPUTFILE) $(objects) $(LDLIBS)

clean:
	rm -f $(objects)
	rm -f $(OUTPUTFILE)