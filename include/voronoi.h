#ifndef VORONOI_H
#define VORONOI_H

#include "class_vector.h"
#include "class_polygon.h"

void compute_voronoi(std::vector<Polygon> &cells, const std::vector<Vector> &points, const std::vector<double> weights);

#endif