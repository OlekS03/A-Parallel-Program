#include "ParallelAStar.h"
#include <queue>
#include <vector>
#include <cmath>

int ParallelAStar::heuristic(int x1, int y1, int x2, int y2) {
    return std::abs(x1 - x2) + std::abs(y1 - y2);
}

bool ParallelAStar::solveSegment(const Graph& graph, int rank, int num_procs) {
    int size = graph.getSize();
    const std::vector<Node>& nodes = graph.getNodes();

    int startX = (rank * (size - 1)) / num_procs;
    int startY = (rank * (size - 1)) / num_procs;
    
    int goalX = ((rank + 1) * (size - 1)) / num_procs;
    int goalY = ((rank + 1) * (size - 1)) / num_procs;

    if (rank == num_procs - 1) {
        goalX = size - 1;
        goalY = size - 1;
    }

    int startIdx = graph.getIndex(startX, startY);
    int goalIdx = graph.getIndex(goalX, goalY);

    if (startIdx == goalIdx) return true;

    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> openSet;
    std::vector<int> gScore(graph.getTotalNodes(), 1e9); 
    
    gScore[startIdx] = 0;
    openSet.push({heuristic(startX, startY, goalX, goalY), startIdx});

    while (!openSet.empty()) {
        int currentIdx = openSet.top().second;
        openSet.pop();

        if (currentIdx == goalIdx) {
            return true; // Local path found
        }

        for (int neighborIdx : nodes[currentIdx].neighbors) {
            int tentative_gScore = gScore[currentIdx] + 1; 

            if (tentative_gScore < gScore[neighborIdx]) {
                gScore[neighborIdx] = tentative_gScore;
                int fScore = tentative_gScore + heuristic(nodes[neighborIdx].x, nodes[neighborIdx].y, goalX, goalY);
                openSet.push({fScore, neighborIdx});
            }
        }
    }
    return false;
}