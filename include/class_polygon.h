#ifndef CLASS_POLYGON_H
#define CLASS_POLYGON_H

#include "class_vector.h"
#include <vector>

class Polygon {
public:
    std::vector<Vector> vertices;
    Polygon(const std::vector<Vector> &Vs = {}) : vertices(Vs) {}

    Vector operator [] (int i) const;
    Vector& operator [](int i);

    size_t size() const;
    double area() const;
    double energy(const Vector &P) const;

    Vector centroid() const;

    void swap(Polygon &other);
    
    bool clip_by_bisector(const Vector &u, const Vector &v, double w1, double w2);
    bool clip_by_edge(const Vector &u, const Vector &v);
    void clip_by_disc(const Vector &C, const double &r);
    void clip_polygon(const Polygon &c);
};


#endif