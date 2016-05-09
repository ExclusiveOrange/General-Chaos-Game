CXX ?= g++
CFLAGS = -O3 -std=c++14 -Wall -Wfatal-errors
RM ?= rm

all: gcg

gcg: gcg.cpp lodepng.o
	$(CXX) $(CFLAGS) -o gcg gcg.cpp lodepng.o

lodepng.o: lodepng.cpp lodepng.h
	$(CXX) $(CFLAGS) -c -o lodepng.o lodepng.cpp

clean:
	$(RM) -f gcg lodepng.o
