CXX = mpicxx
CXXFLAGS = -std=c++11 -Wall -O3
TARGET = maze

OBJS = main.o Graph.o AStar.o BidirectionalAStar.o ParallelAStar.o WaypointAStar.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean