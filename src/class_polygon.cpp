#include "../include/class_polygon.h"

Vector Polygon::operator [] (int i) const {
    return vertices[i];
}
Vector& Polygon::operator [](int i) {
    return vertices[i];
}

size_t Polygon::size() const {
    return vertices.size();
}
double Polygon::area() const {
    double ret = 0;

    for (int i = 1 ; i+1 < vertices.size() ; ++i) {
        const Vector a = vertices[i] - vertices[0];
        const Vector b = vertices[i+1] - vertices[0];

        ret += std::abs(a[0] * b[1] - a[1] * b[0]);
    }
    return ret / 2;
}

void Polygon::swap(Polygon &other) {
    vertices.swap(other.vertices);
}

void Polygon::clip_by_bisector(const Vector &u, const Vector &v, double w1, double w2) {
    std::vector<Vector> ret;

    for (size_t i = 0 ; i < vertices.size() ; ++i) {
        const Vector curr = vertices[i];
        const Vector prev = vertices[i ? i-1 : vertices.size() - 1];

        if (check_inside_voronoi(curr, u, v, w1, w2)) {
            if (!check_inside_voronoi(prev, u, v, w1, w2)) {
                double t = check_intersect_voronoi(prev, curr, u, v, w1, w2);
                Vector P = prev + t * (curr - prev);

                ret.push_back(P);
            }
            ret.push_back(curr);
        } else if (check_inside_voronoi(prev, u, v, w1, w2)) {
            double t = check_intersect_voronoi(prev, curr, u, v, w1, w2);
            Vector P = prev + t * (curr - prev);

            ret.push_back(P);
        }
    }
    vertices.swap(ret);
}
void Polygon::clip_by_edge(const Vector &u, const Vector &v) {
    std::vector<Vector> ret;

    for (size_t i = 0 ; i < vertices.size() ; ++i) {
        const Vector curr = vertices[i];
        const Vector prev = vertices[i ? i-1 : vertices.size() - 1];

        if (check_inside(curr, u, v)) {
            if (!check_inside(prev, u, v)) {
                double t = check_intersect(prev, curr, u, v);
                Vector P = prev + t * (curr - prev);

                ret.push_back(P);
            }
            ret.push_back(curr);
        } else if (check_inside(prev, u, v)) {
            double t = check_intersect(prev, curr, u, v);
            Vector P = prev + t * (curr - prev);

            ret.push_back(P);
        }
    }
    vertices.swap(ret);
}
void Polygon::clip_polygon(const Polygon &c) {
    for (size_t i = 0 ; i < c.size() ; ++i)
        clip_by_edge(c[i], c[i ? i-1 : c.size() - 1]);
}