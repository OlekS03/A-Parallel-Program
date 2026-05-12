#include "BidirectionalAStar.h"
#include <queue>
#include <vector>
#include <cmath>

int BidirectionalAStar::heuristic(int x1, int y1, int x2, int y2) {
    return std::abs(x1 - x2) + std::abs(y1 - y2);
}

bool BidirectionalAStar::solve(const Graph& graph) {
    int size = graph.getSize();
    const std::vector<Node>& nodes = graph.getNodes();
    
    int startIdx = graph.getIndex(0, 0);
    int goalIdx = graph.getIndex(size - 1, size - 1);

    if (startIdx == goalIdx) return true;

    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> openFwd;
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> openBwd;

    // Track costs
    std::vector<int> gFwd(graph.getTotalNodes(), 1e9);
    std::vector<int> gBwd(graph.getTotalNodes(), 1e9);
    
    std::vector<bool> visitedFwd(graph.getTotalNodes(), false);
    std::vector<bool> visitedBwd(graph.getTotalNodes(), false);

    gFwd[startIdx] = 0;
    gBwd[goalIdx] = 0;

    openFwd.push({heuristic(0, 0, size - 1, size - 1), startIdx});
    openBwd.push({heuristic(size - 1, size - 1, 0, 0), goalIdx});

    while (!openFwd.empty() && !openBwd.empty()) {
        int currFwd = openFwd.top().second;
        openFwd.pop();

        if (visitedBwd[currFwd]) return true; 
        visitedFwd[currFwd] = true;

        for (int neighborIdx : nodes[currFwd].neighbors) {
            if (visitedFwd[neighborIdx]) continue;
            
            int tentative_g = gFwd[currFwd] + 1;
            if (tentative_g < gFwd[neighborIdx]) {
                gFwd[neighborIdx] = tentative_g;
                int fScore = tentative_g + heuristic(nodes[neighborIdx].x, nodes[neighborIdx].y, size - 1, size - 1);
                openFwd.push({fScore, neighborIdx});
            }
        }

        int currBwd = openBwd.top().second;
        openBwd.pop();

        if (visitedFwd[currBwd]) return true; 
        visitedBwd[currBwd] = true;

        for (int neighborIdx : nodes[currBwd].neighbors) {
            if (visitedBwd[neighborIdx]) continue;
            
            int tentative_g = gBwd[currBwd] + 1;
            if (tentative_g < gBwd[neighborIdx]) {
                gBwd[neighborIdx] = tentative_g;
                int fScore = tentative_g + heuristic(nodes[neighborIdx].x, nodes[neighborIdx].y, 0, 0);
                openBwd.push({fScore, neighborIdx});
            }
        }
    }
    
    return false;
}