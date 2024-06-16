#ifndef CLASS_EDGE_H
#define CLASS_EDGE_H

#include <algorithm>

class Edge {
public:
    Edge(int u = 0, int v = 0) {
        this->u = u;
        this->v = v;
    }
    bool operator == (const Edge &other) const {
        if (u == other.u && v == other.v)   return true;
        if (u == other.v && v == other.u)   return true;
        return false;
    }
    int u, v;
};

template <>
struct std::hash<Edge> {
    std::size_t operator()(const Edge& e) const {
        std::size_t min_uv = std::min(e.u, e.v);
        std::size_t max_uv = std::max(e.u, e.v);

        return ((min_uv << 20) ^ max_uv);
    }
};

#endif