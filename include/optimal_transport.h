#ifndef OPTIMAL_TRANSPORT_H
#define OPTIMAL_TRANSPORT_H

#include <vector>
#include "../lib/lbfgs.h"
#include "class_vector.h"
#include "voronoi.h"

class OptimalTransport {
public:
    OptimalTransport() {}
    OptimalTransport(const std::vector<Vector>& points = {}, const std::vector<double>& lambdas = {}) {
        _points = points;
        _lambdas = lambdas;
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
        std::vector<Polygon> voronoi;
        
        compute_voronoi(voronoi, _points, x);

        double fx = 0.0;

        for (int i = 0 ; i < n ; ++i) {
            double A = voronoi[i].area();

            g[i] = -(_lambdas[i] - A);

            fx += voronoi[i].energy(_points[i]);
            fx -= A * x[i];
            fx += _lambdas[i] * x[i];
        }
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
        size_t ret = lbfgs((int) _points.size(), weights, &fx, _evaluate, _progress, this, NULL);
    }

    std::vector<Vector> _points;
    std::vector<double> _lambdas;
};

#endif