#include "../include/class_polygon.h"
// #include "../include/constant.h"

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
double Polygon::energy(const Vector &P) const {
    double result = 0;
    
    if (vertices.size() >= 3) {
        for (size_t i = 1 ; i < vertices.size() - 1 ; ++i) {
            const Vector &a = vertices[0];
            const Vector &b = vertices[i];
            const Vector &c = vertices[i+1];

            double local_energy = 
                dot(a - P, a - P) + dot(a - P, b - P) + dot(a - P, c - P) + \
                dot(b - P, b - P) + dot(b - P, c - P) + \
                dot(c - P, c - P);
            
            double local_area = abs(
                (b[0] - a[0]) * (c[1] - a[1]) -
                (b[1] - a[1]) * (c[0] - a[0])
            );

            result += local_energy * local_area / 6.;
        }
    }
    return result;
}

Vector Polygon::centroid() const {
    const double A = area();
    const size_t N = size();
    double Cx = 0;
    double Cy = 0;

    for (size_t i = 0 ; i < N ; ++i) {
        const Vector &u = vertices[i ? i-1 : N-1];
        const Vector &v = vertices[i];

        Cx += (u[0] + v[0]) * (u[0] * v[1] - u[1] * v[0]);
        Cy += (u[1] + v[1]) * (u[0] * v[1] - u[1] * v[0]);
    }
    return Vector(Cx / (6*A), Cy / (6*A));
}

void Polygon::swap(Polygon &other) {
    vertices.swap(other.vertices);
}

bool Polygon::clip_by_bisector(const Vector &u, const Vector &v, double w1, double w2) {
    std::vector<Vector> ret;
    bool change = false;

    for (size_t i = 0 ; i < vertices.size() ; ++i) {
        const Vector curr = vertices[i];
        const Vector prev = vertices[i ? i-1 : vertices.size() - 1];

        if (check_inside_voronoi(curr, u, v, w1, w2)) {
            if (!check_inside_voronoi(prev, u, v, w1, w2)) {
                double t = check_intersect_voronoi(prev, curr, u, v, w1, w2);
                Vector P = prev + t * (curr - prev);
                change = true;
                ret.push_back(P);
            }
            ret.push_back(curr);
        } else if (check_inside_voronoi(prev, u, v, w1, w2)) {
            double t = check_intersect_voronoi(prev, curr, u, v, w1, w2);
            Vector P = prev + t * (curr - prev);
            change = true;
            ret.push_back(P);
        }
    }
    vertices.swap(ret);
    return change;
}
bool Polygon::clip_by_edge(const Vector &u, const Vector &v) {
    std::vector<Vector> ret;
    bool change = false;

    for (size_t i = 0 ; i < vertices.size() ; ++i) {
        const Vector curr = vertices[i];
        const Vector prev = vertices[i ? i-1 : vertices.size() - 1];

        if (check_inside(curr, u, v)) {
            if (!check_inside(prev, u, v)) {
                double t = check_intersect(prev, curr, u, v);
                Vector P = prev + t * (curr - prev);
                change = true;
                ret.push_back(P);
            }
            ret.push_back(curr);
        } else if (check_inside(prev, u, v)) {
            double t = check_intersect(prev, curr, u, v);
            Vector P = prev + t * (curr - prev);
            change = true;
            ret.push_back(P);
        }
    }
    vertices.swap(ret);
    return change;
}
void Polygon::clip_polygon(const Polygon &c) {
    for (size_t i = 0 ; i < c.size() ; ++i)
        clip_by_edge(c[i], c[i ? i-1 : c.size() - 1]);
}