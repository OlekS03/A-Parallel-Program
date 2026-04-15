CXX = g++
CXXFLAGS = -std=c++17 -O2

OBJS = main.o Maze.o AStar.o

maze: $(OBJS)
	$(CXX) $(CXXFLAGS) -o maze $(OBJS)

main.o: main.cpp Maze.h AStar.h
	$(CXX) $(CXXFLAGS) -c main.cpp

Maze.o: Maze.cpp Maze.h
	$(CXX) $(CXXFLAGS) -c Maze.cpp

AStar.o: AStar.cpp AStar.h Maze.h
	$(CXX) $(CXXFLAGS) -c AStar.cpp

clean:
	rm -f *.o maze