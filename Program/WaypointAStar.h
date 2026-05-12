#ifndef WAYPOINTASTAR_H
#define WAYPOINTASTAR_H

#include "Graph.h"

class WaypointAStar {
private:
    static int heuristic(int x1, int y1, int x2, int y2);
    static int getNearestOpenNode(const Graph& graph, int startX, int startY);

public:
    static bool solveSegment(const Graph& graph, int rank, int num_procs);
};

#endif // WAYPOINTASTAR_H