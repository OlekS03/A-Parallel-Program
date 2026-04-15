#ifndef MAZE_H
#define MAZE_H

#include <vector>

class Maze {
public:
    int width, height;
    std::vector<std::vector<int>> grid;

    Maze(int w, int h);

    void generate();
    bool isWalkable(int x, int y) const;

private:
    void carve(int x, int y);
};

#endif