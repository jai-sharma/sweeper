CXX=g++
BIN=sweeper

LINK_FLAGS=-lsfml-system -lsfml-window -lsfml-graphics
COMPILER_FLAGS=-std=c++11 -Wall -Werror -pedantic-errors

all:
	$(CXX) *.cpp $(COMPILER_FLAGS) $(LINK_FLAGS) -o $(BIN)
