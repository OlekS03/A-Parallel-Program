#include "Graph.h"
#include <stack>
#include <random>
#include <utility>
#include <iostream>

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

#include <iostream> // Make sure this is included at the top of Graph.cpp

// ... (Your existing Graph.cpp code) ...

void Graph::printMaze() const {
    // Print the top outer boundary
    for (int x = 0; x < size * 2 + 1; ++x) std::cout << "██";
    std::cout << "\n";

    for (int y = 0; y < size; ++y) {
        // Left boundary for the current row
        std::cout << "██"; 
        
        // Print the rooms (nodes) and the Right doors (horizontal connections)
        for (int x = 0; x < size; ++x) {
            int currIdx = getIndex(x, y);
            
            // Draw the room itself
            if (x == 0 && y == 0) std::cout << "SS";      // Start
            else if (x == size - 1 && y == size - 1) std::cout << "EE"; // End
            else std::cout << "  ";                       // Open Room
            
            // Draw the Right door
            bool rightConnected = false;
            for (int n : nodes[currIdx].neighbors) {
                if (n == getIndex(x + 1, y)) rightConnected = true;
            }
            
            if (x < size - 1) {
                if (rightConnected) std::cout << "  "; // Open passage
                else std::cout << "██";                // Wall block
            }
        }
        std::cout << "██\n"; // Right boundary
        
        // Print the Down doors (vertical connections) and corner pillars
        if (y < size - 1) {
            std::cout << "██"; // Left boundary
            for (int x = 0; x < size; ++x) {
                int currIdx = getIndex(x, y);
                
                // Draw the Down door
                bool downConnected = false;
                for (int n : nodes[currIdx].neighbors) {
                    if (n == getIndex(x, y + 1)) downConnected = true;
                }
                
                if (downConnected) std::cout << "  "; // Open passage
                else std::cout << "██";               // Wall block
                
                // Draw the corner pillar between rooms
                if (x < size - 1) std::cout << "██";  
            }
            std::cout << "██\n"; // Right boundary
        }
    }
    
    // Print the bottom outer boundary
    for (int x = 0; x < size * 2 + 1; ++x) std::cout << "██";
    std::cout << "\n";
}