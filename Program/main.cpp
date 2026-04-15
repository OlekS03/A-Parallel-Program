#include "Maze.h"
#include "AStar.h"
#include <iostream>
#include <chrono>
#include <cstdlib>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "Usage: ./maze size runs\n";
        std::cout << "Example: ./maze 100 100\n";
        return 1;
    }

    int size = atoi(argv[1]);
    int runs = atoi(argv[2]);

    // Ensure odd dimensions (critical for maze correctness)
    if (size % 2 == 0) size++;

    int width = size;
    int height = size;

    srand(time(nullptr));

    double totalTime = 0.0;
    int solvedCount = 0;

    for (int i = 0; i < runs; i++) {
        Maze maze(width, height);
        maze.generate();

        auto start = std::chrono::high_resolution_clock::now();

        bool solved = AStar::solve(maze);

        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::milli> duration = end - start;
        totalTime += duration.count();

        if (solved) {
            solvedCount++;
        } else {
            std::cout << "Maze " << i << " not solvable!\n";
        }
    }

    std::cout << "\n==== Results ====\n";
    std::cout << "Maze size: " << width << "x" << height << "\n";
    std::cout << "Runs: " << runs << "\n";
    std::cout << "Solved: " << solvedCount << "/" << runs << "\n";
    std::cout << "Average solve time: "
              << (totalTime / runs)
              << " ms\n";

    return 0;
}