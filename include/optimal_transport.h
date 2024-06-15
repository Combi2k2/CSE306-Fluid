#ifndef OPTIMAL_TRANSPORT_H
#define OPTIMAL_TRANSPORT_H

#include <vector>
#include "../lib/lbfgs.h"
#include "class_vector.h"
#include "constant.h"

#include "voronoi.h"

class OptimalTransport {
public:
    OptimalTransport() {}
    OptimalTransport(const std::vector<Vector> &points, const std::vector<double>& lambdas) {
        this->points  = points;
        this->lambdas = lambdas;
        A.resize(points.size());
        E.resize(points.size());
    }

    static lbfgsfloatval_t _evaluate(
        void *instance,
        const lbfgsfloatval_t *x,
        lbfgsfloatval_t *g,
        const int n,
        const lbfgsfloatval_t step
        )
    {
        return reinterpret_cast<OptimalTransport*>(instance)->evaluate(x, g, n, step);
    }

    lbfgsfloatval_t evaluate(
        const lbfgsfloatval_t *x,
        lbfgsfloatval_t *g,
        const int n,
        const lbfgsfloatval_t step
        )
    {
        compute_voronoi(voronoi, points, x);

        double fx = 0.0;
        double volume_fluid = 0.0;

#pragma omp parallel for schedule(dynamic)
        for (int i = 0 ; i < n-1 ; ++i) {
            if (x[i] <= x[n-1]) {
                voronoi[i] = Polygon();
            } else {
                voronoi[i].clip_by_disc(points[i], sqrt(x[i] - x[n-1]));
            }

            E[i] = voronoi[i].energy(points[i]);
            A[i] = voronoi[i].area();
            g[i] = -(lambdas[i] - A[i]);
        }
        for (int i = 0 ; i < n-1 ; ++i) {
            fx += E[i];
            fx -= A[i] * x[i];
            fx += lambdas[i] * x[i];
            volume_fluid += A[i];
        }
        fx += x[n-1] * (VOLUME_AIR - (1.0 - volume_fluid));
        g[n-1] = -(VOLUME_AIR - (1.0 - volume_fluid));
        
        return -fx;
    }

    static int _progress(
        void *instance,
        const lbfgsfloatval_t *x,
        const lbfgsfloatval_t *g,
        const lbfgsfloatval_t fx,
        const lbfgsfloatval_t xnorm,
        const lbfgsfloatval_t gnorm,
        const lbfgsfloatval_t step,
        int n,
        int k,
        int ls
        )
    {
        return reinterpret_cast<OptimalTransport*>(instance)->progress(x, g, fx, xnorm, gnorm, step, n, k, ls);
    }

    int progress(
        const lbfgsfloatval_t *x,
        const lbfgsfloatval_t *g,
        const lbfgsfloatval_t fx,
        const lbfgsfloatval_t xnorm,
        const lbfgsfloatval_t gnorm,
        const lbfgsfloatval_t step,
        int n,
        int k,
        int ls
        )
    {
        //printf("Iteration %d:\n", k);
        //printf("  fx = %f, x[0] = %f, x[1] = %f\n", fx, x[0], x[1]);
        //printf("  xnorm = %f, gnorm = %f, step = %f\n", xnorm, gnorm, step);
        //printf("\n");
        return 0;
    }

    void solve(double *weights) {
        double fx = 0.0;
        int ret = lbfgs((int)points.size()+1, weights, &fx, _evaluate, _progress, this, NULL);
    }

    std::vector<Vector> points;
    std::vector<double> lambdas;
    std::vector<Polygon> voronoi;
    std::vector<double> A, E;
};

#endif