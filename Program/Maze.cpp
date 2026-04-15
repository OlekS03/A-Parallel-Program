#include "Maze.h"
#include <cstdlib>
#include <ctime>

Maze::Maze(int w, int h) : width(w), height(h) {
    grid.resize(height, std::vector<int>(width, 1));
}

bool Maze::isWalkable(int x, int y) const {
    return x >= 0 && y >= 0 && x < width && y < height && grid[y][x] == 0;
}

void Maze::generate() {
    // Fill walls
    for (auto &row : grid)
        std::fill(row.begin(), row.end(), 1);

    carve(1, 1);
}

void Maze::carve(int x, int y) {
    grid[y][x] = 0;

    int dirs[4][2] = {{2,0},{-2,0},{0,2},{0,-2}};

    // shuffle directions
    for (int i = 0; i < 4; i++) {
        int r = rand() % 4;
        std::swap(dirs[i], dirs[r]);
    }

    for (auto &d : dirs) {
        int nx = x + d[0];
        int ny = y + d[1];

        if (nx > 0 && ny > 0 && nx < width-1 && ny < height-1 && grid[ny][nx] == 1) {
            grid[y + d[1]/2][x + d[0]/2] = 0;
            carve(nx, ny);
        }
    }
}