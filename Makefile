SOURCES=src/*.cpp optim/*.cpp main.cpp
HEADERS=include/*.h optim/*.h

CFLAGS=-std=c++20 -O3 -fopenmp -lpthread

fluid_sim: $(SOURCES) $(HEADERS)
	g++ lib/lbfgs.c $(SOURCES) -o fluid_sim $(CFLAGS)

clean:
	rm -f fluid_sim