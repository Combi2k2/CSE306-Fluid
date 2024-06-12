#include "../include/voronoi.h"

void compute_voronoi(std::vector<Polygon> &cells, const std::vector<Vector> &points, const std::vector<double> weights) {
    std::vector<Vector> square = {
        Vector(0, 0),
        Vector(1, 0),
        Vector(1, 1),
        Vector(0, 1)
    };
    cells.resize(points.size());

    for (size_t i = 0 ; i < points.size() ; ++i) {
        Polygon S(square);

        for (size_t j = 0 ; j < points.size() ; ++j)
            if (i != j)
                S.clip_by_bisector(points[i], points[j], weights[i], weights[j]);
        
        cells[i].swap(S);
    }
}