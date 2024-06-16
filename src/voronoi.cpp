#include "../include/voronoi.h"
#include "../optim/optim.h"

#include <algorithm>
#include <iostream>

void compute_voronoi(std::vector<Polygon> &cells, const std::vector<Vector> &points, const double *weights) {
    std::vector<Vector> square = {
        Vector(0, 0),
        Vector(1, 0),
        Vector(1, 1),
        Vector(0, 1)
    };
    cells.resize(points.size());

#ifndef KDTREE_OPTIM
#pragma omp parallel for schedule(dynamic, 1)
    for (size_t i = 0 ; i < points.size() ; ++i) {
        Polygon S(square);

        for (size_t j = 0 ; j < points.size() ; ++j)
            if (i != j)
                S.clip_by_bisector(points[i], points[j], weights[i], weights[j]);
        
        cells[i].swap(S);
    }
#else
    double max_weight = *std::max_element(weights, weights + points.size());

    std::vector<Vector> points_prime;

    for (size_t i = 0 ; i < points.size() ; ++i)
        points_prime.push_back(Vector(
            points[i][0],
            points[i][1],
            sqrt(max_weight - weights[i])
        ));
    
    Kd_Tree kdtree(3, points_prime);

#pragma omp parallel for schedule(dynamic, 1)
    for (size_t i = 0 ; i < points.size() ; ++i) {
        Polygon S(square);

        std::vector<int> indices;

        int prev_k = 0;
        int curr_k = KDTREE_K;

        for (; prev_k < points.size() ; curr_k += KDTREE_K) {
            kdtree.knnSearch(indices, points_prime[i], curr_k);
            bool change = true;

            for (; prev_k < indices.size() ; ++prev_k) {
                int idx = indices[prev_k];
                if (idx == i)
                    continue;
                
                if ((points_prime[i] - points_prime[idx]).norm2() > 4 * S.maxDist(points[i])) {
                    change = false;
                    break;
                }
                S.clip_by_bisector(points[i], points[idx], weights[i], weights[idx]);
            }
            if (!change)
                break;
        }
        cells[i].swap(S);
    }
#endif
}