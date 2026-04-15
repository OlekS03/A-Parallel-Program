#ifndef ASTAR_H
#define ASTAR_H

#include "Maze.h"
#include <vector>

struct Node {
    int x, y;
    int g, h;
    Node* parent;
};

class AStar {
public:
    static bool solve(const Maze& maze);
};

#endif