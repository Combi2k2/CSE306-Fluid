#ifndef KDTREE_H
#define KDTREE_H

#include "../include/class_vector.h"
#include "../include/constant.h"

#include <vector>
#include <queue>

struct Kd_Node {
    int idx;            // index to the original point
    int axis;           // dimension's axis
    Kd_Node *child[2];  // pointers to the child nodes

    Kd_Node(int idx = -1, int dim = -1, Kd_Node *left = nullptr, Kd_Node *right = nullptr);
    ~Kd_Node();
};

class Kd_Tree {
private:
    std::vector<Vector> _points;

    Kd_Node *_root = nullptr;
    int _ndim;

    Kd_Node *buildRecursive(int *indices, int npoints, int depth);

    void nnSearchRecursive(const Kd_Node *node, const Vector& P, int &ret, double &minDist) const;
    void knnSearchRecursive(const Kd_Node *node, const Vector& P, std::priority_queue<std::pair<double, int> > &knnQueue, int k) const;
    void radiusSearchRecursive(const Kd_Node *node, const Vector &P, std::vector<int>& indices, double r) const;

public:
    Kd_Tree(int ndim, const std::vector<Vector> &points);
    ~Kd_Tree();

    int nnSearch(const Vector& P, double &minDist) const;

    void knnSearch(std::vector<int> &result, const Vector& P, int k) const;
    void radiusSearch(std::vector<int> &result, const Vector& P, double r) const;
public:
};


#endif