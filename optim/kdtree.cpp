#include "kdtree.h"

#include <algorithm>
#include <numeric>
#include <iostream>

Kd_Node::Kd_Node(int idx, int dim, Kd_Node *left, Kd_Node *right) {
    this->idx = idx;
    this->axis = dim;
    this->child[0] = left;
    this->child[1] = right;
}
Kd_Node::~Kd_Node() {
    delete child[0];
    delete child[1];
}

Kd_Tree::Kd_Tree(int ndim, const std::vector<Vector> &points) {
    _ndim = ndim;
    _points = points;

    std::vector<int> indices(points.size());
    std::iota(indices.begin(), indices.end(), 0);

    _root = buildRecursive(indices.data(), (int)points.size(), 0);
}
Kd_Tree::~Kd_Tree() {
    delete _root;
}

Kd_Node* Kd_Tree::buildRecursive(int *indices, int npoints, int depth) {
    if (npoints <= 0)
        return nullptr;

    const int axis = depth % _ndim;
    const int mid = (npoints - 1) / 2;

    std::nth_element(indices, indices + mid, indices + npoints, [&](int lhs, int rhs) {
        return _points[lhs][axis] < _points[rhs][axis];
    });
    Kd_Node *lchild = buildRecursive(indices, mid, depth + 1);
    Kd_Node *rchild = buildRecursive(indices + mid + 1, npoints - mid - 1, depth + 1);

    return new Kd_Node(indices[mid], axis, lchild, rchild);
}

int Kd_Tree::nnSearch(const Vector& P, double &minDist) const {
    int result = -1;
    minDist = std::numeric_limits<double>::max();

    nnSearchRecursive(_root, P, result, minDist);

    return result;
}

void Kd_Tree::knnSearch(std::vector<int> &result, const Vector& P, int k) const {
    std::priority_queue<std::pair<double, int> > knnQueue;
    knnSearchRecursive(_root, P, knnQueue, k);

    result.clear();

    while (knnQueue.size()) {
        result.push_back(knnQueue.top().second);
        knnQueue.pop();
    }
    std::reverse(result.begin(), result.end());
}
void Kd_Tree::radiusSearch(std::vector<int> &result, const Vector& P, double r) const {
    result.clear();
    radiusSearchRecursive(_root, P, result, r);
}

void Kd_Tree::nnSearchRecursive(const Kd_Node *node, const Vector& P, int &ret, double &minDist) const {
    if (node == nullptr)
        return;
    
    const Vector &train = _points[node->idx];
    const double dist = (P - train).norm();

    if (minDist > dist) {
        minDist = dist;
        ret = node->idx;
    }
    const int axis = node->axis;
    const int dir = P[axis] < train[axis] ? 0 : 1;

    nnSearchRecursive(node->child[dir], P, ret, minDist);

    const double diff = abs(P[axis] - train[axis]);

    if (diff < minDist)
        nnSearchRecursive(node->child[!dir], P, ret, minDist);
}
void Kd_Tree::knnSearchRecursive(const Kd_Node *node, const Vector& P, std::priority_queue<std::pair<double, int> > &knnQueue, int k) const {
    if (node == nullptr)
        return;
    
    const Vector &train = _points[node->idx];
    const double dist = (P - train).norm();

    knnQueue.push(std::make_pair(dist, node->idx));

    if (knnQueue.size() > k)
        knnQueue.pop();

    const int axis = node->axis;
    const int dir = P[axis] < train[axis] ? 0 : 1;

    knnSearchRecursive(node->child[dir], P, knnQueue, k);

    const double diff = fabs(P[axis] - train[axis]);

    if (knnQueue.size() < k || diff < knnQueue.top().first)
        knnSearchRecursive(node->child[!dir], P, knnQueue, k);
}
void Kd_Tree::radiusSearchRecursive(const Kd_Node *node, const Vector &P, std::vector<int>& indices, double r) const {
    if (node == nullptr)
        return;
    
    const Vector &train = _points[node->idx];
    const double dist = (P - train).norm();

    if (dist < r)
        indices.push_back(node->idx);

    const int axis = node->axis;
    const int dir = P[axis] < train[axis] ? 0 : 1;

    radiusSearchRecursive(node->child[dir], P, indices, r);

    const double diff = fabs(P[axis] - train[axis]);

    if (diff < r)
        radiusSearchRecursive(node->child[!dir],P,  indices, r);
}