#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include "Node.h"

class Graph {
private:
    int size;
    int totalNodes;
    std::vector<Node> nodes;

public:
    Graph(int size);
    
    void generatePerfectMaze(unsigned int seed);
    void generateGameMap(unsigned int seed, double obstacleDensity);
    void generateGuaranteedGameMap(unsigned int seed, double openFactor);
    
    // NEW: Prints a visual representation of the maze to the console
    void printMaze() const;
    
    const std::vector<Node>& getNodes() const;
    int getSize() const;
    int getTotalNodes() const;
    int getIndex(int x, int y) const;
};

#endif // GRAPH_H