#include "AStar.h"
#include <queue>
#include <vector>
#include <cmath>

int AStar::heuristic(int x1, int y1, int x2, int y2) {
    return std::abs(x1 - x2) + std::abs(y1 - y2);
}

bool AStar::solve(const Graph& graph) {
    int size = graph.getSize();
    const std::vector<Node>& nodes = graph.getNodes();
    
    int startIdx = graph.getIndex(0, 0);
    int goalIdx = graph.getIndex(size - 1, size - 1);

    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> openSet;

    std::vector<int> gScore(graph.getTotalNodes(), 1e9); 
    gScore[startIdx] = 0;

    openSet.push({heuristic(0, 0, size - 1, size - 1), startIdx});

    while (!openSet.empty()) {
        int currentIdx = openSet.top().second;
        openSet.pop();

        if (currentIdx == goalIdx) {
            return true;
        }

        const Node& currentNode = nodes[currentIdx];

        // Check all connected neighbors in the graph
        for (int neighborIdx : currentNode.neighbors) {
            int tentative_gScore = gScore[currentIdx] + 1; 

            if (tentative_gScore < gScore[neighborIdx]) {
                gScore[neighborIdx] = tentative_gScore;
                
                const Node& neighborNode = nodes[neighborIdx];
                int fScore = tentative_gScore + heuristic(neighborNode.x, neighborNode.y, size - 1, size - 1);
                
                openSet.push({fScore, neighborIdx});
            }
        }
    }
    return false;
}