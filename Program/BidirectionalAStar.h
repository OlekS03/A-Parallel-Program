#ifndef BIDIRECTIONAL_ASTAR_H
#define BIDIRECTIONAL_ASTAR_H

#include "Graph.h"

class BidirectionalAStar {
private:
    static int heuristic(int x1, int y1, int x2, int y2);

public:
    static bool solve(const Graph& graph);
};

#endif // BIDIRECTIONAL_ASTAR_H