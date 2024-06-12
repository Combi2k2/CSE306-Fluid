#include "include/class_vector.h"
#include "include/class_polygon.h"
#include "include/voronoi.h"
#include "include/utils.h"

#include <iostream>

int main() {
    const int n_points = 2000;
    std::vector<Vector> points;
    std::vector<double> weights;
    std::vector<double> balance_weights;

    for (int i = 0; i < n_points; i++) {
        double x = urand();
        double y = urand();
        double w = urand();

        points.push_back(Vector(x, y));
        weights.push_back(w / 4);
        balance_weights.push_back(1.0);
    }
    std::vector<Polygon> Voronoi1;
    std::vector<Polygon> Voronoi2;

    compute_voronoi(Voronoi1, points, balance_weights);
    compute_voronoi(Voronoi2, points, weights);

    save_svg(Voronoi1, points, "balance_image.svg");
    save_svg(Voronoi2, points, "weighted_image.svg");
}