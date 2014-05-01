CC=g++
CFLAGS=-O0 -ggdb -DDEBUG -c -std=c++11
# CFLAGS=-O2 -c -std=c++11
LDFLAGS=-lpthread
CPP_SOURCES=$(wildcard *.cpp game_objects/*.cpp)
OBJECTS=$(CPP_SOURCES:.cpp=.o)
EXECUTABLE=bomberman_server

all: $(CPP_SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

$(OBJECTS):
	$(CC) $(CFLAGS) $(@:.o=.cpp) -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
