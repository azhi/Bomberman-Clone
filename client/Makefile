CC=g++
CFLAGS=-c -std=c++11
LDFLAGS=-lSDL2
CPP_SOURCES=$(wildcard *.cpp sdl/*.cpp game_objects/*.cpp controllers/*.cpp)
OBJECTS=$(CPP_SOURCES:.cpp=.o)
EXECUTABLE=bomberman

all: $(CPP_SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

$(OBJECTS):
	$(CC) $(CFLAGS) $(@:.o=.cpp) -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
