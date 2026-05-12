#include "WaypointAStar.h"
#include <queue>
#include <vector>
#include <cmath>

int WaypointAStar::heuristic(int x1, int y1, int x2, int y2) {
    return std::abs(x1 - x2) + std::abs(y1 - y2);
}

int WaypointAStar::getNearestOpenNode(const Graph& graph, int startX, int startY) {
    int size = graph.getSize();
    const std::vector<Node>& nodes = graph.getNodes();
    
    int startIdx = graph.getIndex(startX, startY);
    // If it has connections, it's not a wall
    if (!nodes[startIdx].neighbors.empty() || (startX == 0 && startY == 0) || (startX == size-1 && startY == size-1)) {
        return startIdx; 
    }

    // BFS to find the closest open grass tile
    std::queue<int> q;
    std::vector<bool> visited(graph.getTotalNodes(), false);
    
    q.push(startIdx);
    visited[startIdx] = true;

    int dx[] = {1, -1, 0, 0, 1, 1, -1, -1};
    int dy[] = {0, 0, 1, -1, 1, -1, 1, -1};

    while (!q.empty()) {
        int curr = q.front();
        q.pop();

        if (!nodes[curr].neighbors.empty()) {
            return curr; 
        }

        int cx = nodes[curr].x;
        int cy = nodes[curr].y;

        for (int i = 0; i < 8; i++) {
            int nx = cx + dx[i];
            int ny = cy + dy[i];
            
            if (nx >= 0 && nx < size && ny >= 0 && ny < size) {
                int nextIdx = graph.getIndex(nx, ny);
                if (!visited[nextIdx]) {
                    visited[nextIdx] = true;
                    q.push(nextIdx);
                }
            }
        }
    }
    return startIdx; 
}

bool WaypointAStar::solveSegment(const Graph& graph, int rank, int num_procs) {
    int size = graph.getSize();
    const std::vector<Node>& nodes = graph.getNodes();

    int targetStartX = (rank * (size - 1)) / num_procs;
    int targetStartY = (rank * (size - 1)) / num_procs;
    
    int targetGoalX = ((rank + 1) * (size - 1)) / num_procs;
    int targetGoalY = ((rank + 1) * (size - 1)) / num_procs;

    if (rank == num_procs - 1) {
        targetGoalX = size - 1;
        targetGoalY = size - 1;
    }

    int startIdx = getNearestOpenNode(graph, targetStartX, targetStartY);
    int goalIdx = getNearestOpenNode(graph, targetGoalX, targetGoalY);

    if (startIdx == goalIdx) return true;

    int actualGoalX = nodes[goalIdx].x;
    int actualGoalY = nodes[goalIdx].y;

    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> openSet;
    std::vector<int> gScore(graph.getTotalNodes(), 1e9); 
    
    gScore[startIdx] = 0;
    openSet.push({heuristic(nodes[startIdx].x, nodes[startIdx].y, actualGoalX, actualGoalY), startIdx});

    while (!openSet.empty()) {
        int currentIdx = openSet.top().second;
        openSet.pop();

        if (currentIdx == goalIdx) {
            return true; 
        }

        for (int neighborIdx : nodes[currentIdx].neighbors) {
            int tentative_gScore = gScore[currentIdx] + 1; 

            if (tentative_gScore < gScore[neighborIdx]) {
                gScore[neighborIdx] = tentative_gScore;
                int fScore = tentative_gScore + heuristic(nodes[neighborIdx].x, nodes[neighborIdx].y, actualGoalX, actualGoalY);
                openSet.push({fScore, neighborIdx});
            }
        }
    }
    return false;
}