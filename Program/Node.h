#ifndef NODE_H
#define NODE_H

#include <vector>

struct Node {
    int id;
    int x;
    int y;
    std::vector<int> neighbors;
};

#endif // NODE_H