#ifndef ASTAR_H
#define ASTAR_H

#include "Graph.h"

class AStar {
private:
    static int heuristic(int x1, int y1, int x2, int y2);

public:
    static bool solve(const Graph& graph);
};

#endif // ASTAR_H