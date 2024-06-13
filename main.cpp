#include "include/class_vector.h"
#include "include/class_polygon.h"
#include "include/optimal_transport.h"
#include "include/voronoi.h"
#include "include/utils.h"

#include <numeric>
#include <iostream>

int main() {
    const int n_points = 2000;

    std::vector<Vector> points;
    std::vector<double> lambdas;

    double weights[n_points];
    double balance_weights[n_points];

    for (int i = 0; i < n_points; i++) {
        double x = urand();
        double y = urand();
        double w = urand();

        points.push_back(Vector(x, y));
        lambdas.push_back(exp(-(points.back() - Vector(0.5, 0.5)).norm2() / 0.02));
        weights[i] = w / 2;
        balance_weights[i] = 1.0;
    }
    double sum_lambda = std::accumulate(lambdas.begin(), lambdas.end(), 0.0);

    for (double &lambda : lambdas)
        lambda /= sum_lambda;

    OptimalTransport solver(points, lambdas);
    solver.solve(weights);

    std::vector<Polygon> Voronoi1;
    std::vector<Polygon> Voronoi2;

    compute_voronoi(Voronoi1, points, balance_weights);
    compute_voronoi(Voronoi2, points, weights);

    save_svg(Voronoi1, points, "balance_image.svg");
    save_svg(Voronoi2, points, "weighted_image.svg");
}