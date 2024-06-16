#include "include/class_vector.h"
#include "include/class_polygon.h"
#include "include/optimal_transport.h"
#include "include/voronoi.h"
#include "include/utils.h"

#include <numeric>
#include <iostream>

std::vector<Vector> velocity;
std::vector<double> weights;

OptimalTransport solver;

void init(int n) {
    velocity.assign(n, Vector(0, 0));
    weights.assign(n+1, 1.0);
    weights.back() = 0.99;
    
    std::vector<Vector> points;
    std::vector<double> lambdas;

    for (int i = 0 ; i < n ; ++i) {
        points.push_back(Vector(urand(), urand()));
        lambdas.push_back(1.0 * VOLUME_FLUID / n);
    }
    solver = OptimalTransport(points, lambdas);
}
void step(int frameid) {
    int n = velocity.size();

    solver.solve(&weights[0]);
    save_frame(solver.voronoi, "images/fluid_", frameid);

#pragma omp parallel for schedule(dynamic)
    for (size_t i = 0 ; i < n ; ++i) {
        Vector centroid = solver.voronoi[i].area() ? solver.voronoi[i].centroid() : solver.points[i];
        Vector Fgravity = Vector(0, -9.80665) * MASS_FLUID;
        Vector Fspring  = 1./(EPS * EPS) * (centroid - solver.points[i]);
        Vector forces = Fgravity + Fspring;

        velocity[i] += DELTAT * (forces / MASS_FLUID);

        Vector npos = solver.points[i] + DELTAT * velocity[i];

        if (npos[0] < 0 || npos[0] > 1 ||
            npos[1] < 0 || npos[1] > 1)
                velocity[i] *= -1;
            
        solver.points[i] += DELTAT * velocity[i];
    }
}

int main() {
    const int N_PARTICLES = 500;
    const int N_FRAMES = 400;
    // const int n_points = 2000;

    init(N_PARTICLES);

    for (int i = 0 ; i < N_FRAMES ; ++i)
        step(i);

    // std::vector<Vector> points;
    // std::vector<double> lambdas;

    // double weights[n_points];
    // double balance_weights[n_points];

    // for (int i = 0; i < n_points; i++) {
    //     double x = urand();
    //     double y = urand();
    //     double w = urand();

    //     points.push_back(Vector(x, y));
    //     lambdas.push_back(exp(-(points.back() - Vector(0.5, 0.5)).norm2() / 0.02));
    //     weights[i] = w/2;
    //     balance_weights[i] = 1.0;
    // }
    // double sum_lambda = std::accumulate(lambdas.begin(), lambdas.end(), 0.0);

    // for (double &lambda : lambdas)
    //     lambda /= sum_lambda;

    // OptimalTransport solver(points, lambdas);
    // solver.solve(weights);

    // std::vector<Polygon> Voronoi1;
    // std::vector<Polygon> Voronoi2;

    // compute_voronoi(Voronoi1, points, balance_weights);
    // compute_voronoi(Voronoi2, points, weights);

    // save_svg(Voronoi1, points, "voronoi_diagram.svg");
    // save_svg(Voronoi2, points, "power_diagram.svg");
}