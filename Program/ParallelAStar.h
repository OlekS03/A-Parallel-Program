#ifndef PARALLEL_ASTAR_H
#define PARALLEL_ASTAR_H

#include "Graph.h"

class ParallelAStar {
private:
    static int heuristic(int x1, int y1, int x2, int y2);

public:
    static bool solveSegment(const Graph& graph, int rank, int num_procs);
};

#endif // PARALLEL_ASTAR_H