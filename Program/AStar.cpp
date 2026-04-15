#include "AStar.h"
#include <queue>
#include <cmath>
#include <unordered_set>

struct Compare {
    bool operator()(Node* a, Node* b) {
        return (a->g + a->h) > (b->g + b->h);
    }
};

static int heuristic(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

bool AStar::solve(const Maze& maze) {
    int goalX = maze.width - 2;
    int goalY = maze.height - 2;

    std::priority_queue<Node*, std::vector<Node*>, Compare> open;
    std::vector<std::vector<bool>> visited(maze.height, std::vector<bool>(maze.width, false));

    Node* start = new Node{1, 1, 0, heuristic(1,1,goalX,goalY), nullptr};
    open.push(start);

    while (!open.empty()) {
        Node* current = open.top();
        open.pop();

        if (current->x == goalX && current->y == goalY) {
            delete current;
            return true;
        }

        if (visited[current->y][current->x]) {
            delete current;
            continue;
        }

        visited[current->y][current->x] = true;

        int dirs[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};

        for (auto &d : dirs) {
            int nx = current->x + d[0];
            int ny = current->y + d[1];

            if (maze.isWalkable(nx, ny) && !visited[ny][nx]) {
                Node* neighbor = new Node{
                    nx, ny,
                    current->g + 1,
                    heuristic(nx, ny, goalX, goalY),
                    nullptr
                };
                open.push(neighbor);
            }
        }

        delete current;
    }

    return false;
}