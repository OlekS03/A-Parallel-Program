#include "Graph.h"
#include <stack>
#include <random>
#include <utility>

Graph::Graph(int size) : size(size), totalNodes(size * size) {
    nodes.resize(totalNodes);
    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            int id = getIndex(x, y);
            nodes[id] = {id, x, y, {}};
        }
    }
}

int Graph::getIndex(int x, int y) const {
    return y * size + x;
}

const std::vector<Node>& Graph::getNodes() const {
    return nodes;
}

int Graph::getSize() const {
    return size;
}

int Graph::getTotalNodes() const {
    return totalNodes;
}

void Graph::generateGuaranteedGameMap(unsigned int seed, double openFactor) {
    for (auto& node : nodes) {
        node.neighbors.clear();
    }

    std::vector<bool> visited(totalNodes, false);
    std::stack<std::pair<int, int>> s;
    std::mt19937 gen(seed);

    int startX = 0, startY = 0;
    s.push({startX, startY});
    visited[getIndex(startX, startY)] = true;

    int dx[] = {1, -1, 0, 0};
    int dy[] = {0, 0, 1, -1};

    while (!s.empty()) {
        int cx = s.top().first;
        int cy = s.top().second;
        int currIdx = getIndex(cx, cy);

        std::vector<std::pair<int, int>> unvisited_neighbors;
        for (int i = 0; i < 4; i++) {
            int nx = cx + dx[i];
            int ny = cy + dy[i];

            if (nx >= 0 && nx < size && ny >= 0 && ny < size) {
                if (!visited[getIndex(nx, ny)]) {
                    unvisited_neighbors.push_back({nx, ny});
                }
            }
        }

        if (!unvisited_neighbors.empty()) {
            std::uniform_int_distribution<> dist(0, unvisited_neighbors.size() - 1);
            auto next = unvisited_neighbors[dist(gen)];
            int nx = next.first;
            int ny = next.second;
            int nextIdx = getIndex(nx, ny);

            nodes[currIdx].neighbors.push_back(nextIdx);
            nodes[nextIdx].neighbors.push_back(currIdx);

            visited[nextIdx] = true;
            s.push({nx, ny});
        } else {
            s.pop(); 
        }
    }

    std::uniform_real_distribution<> dis(0.0, 1.0);
    int dx_add[] = {1, 0};
    int dy_add[] = {0, 1}; 
    
    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            int currIdx = getIndex(x, y);
            for (int i = 0; i < 2; i++) {
                int nx = x + dx_add[i];
                int ny = y + dy_add[i];
                
                if (nx < size && ny < size) {
                    int nextIdx = getIndex(nx, ny);
                    
                    bool connected = false;
                    for(int n : nodes[currIdx].neighbors) {
                        if(n == nextIdx) { connected = true; break; }
                    }
                    
                    if(!connected && dis(gen) < openFactor) {
                        nodes[currIdx].neighbors.push_back(nextIdx);
                        nodes[nextIdx].neighbors.push_back(currIdx);
                    }
                }
            }
        }
    }
}