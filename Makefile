SOURCES=src/*.cpp main.cpp
HEADERS=include/*.h

CFLAGS=-std=c++17 -O3 -fopenmp -lpthread

fluid_sim: $(SOURCES) $(HEADERS)
	g++ $(SOURCES) -o fluid_sim $(CFLAGS)

clean:
	rm -f fluid_sim