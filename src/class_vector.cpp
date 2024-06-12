#include "../include/class_vector.h"

Vector operator + (const Vector &a, const Vector &b) {
    return Vector(a[0] + b[0], a[1] + b[1], a[2] + b[2]);
}
Vector operator - (const Vector &a, const Vector &b) {
    return Vector(a[0] - b[0], a[1] - b[1], a[2] - b[2]);
}
Vector operator * (const Vector &a, const Vector &b) {
    return Vector(a[0] * b[0], a[1] * b[1], a[2] * b[2]);
}
Vector operator * (const double &a, const Vector &b) {
    return Vector(a * b[0], a * b[1], a * b[2]);
}
Vector operator * (const Vector &a, const double &b) {
    return Vector(a[0] * b, a[1] * b, a[2] * b);
}
Vector operator / (const Vector &a, const double &b) {
    return Vector(a[0] / b, a[1] / b, a[2] / b);
}
double dot(const Vector& a, const Vector& b) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

void operator += (Vector &a, const Vector &b) {
    a[0] += b[0];
    a[1] += b[1];
    a[2] += b[2];
}
void operator -= (Vector &a, const Vector &b) {
    a[0] -= b[0];
    a[1] -= b[1];
    a[2] -= b[2];
}
void operator *= (Vector &a, const Vector &b) {
    a[0] *= b[0];
    a[1] *= b[1];
    a[2] *= b[2];
}
void operator *= (Vector &a, const double &b) {
    a[0] *= b;
    a[1] *= b;
    a[2] *= b;
}
void operator /= (Vector &a, const double &b) {
    a[0] /= b;
    a[1] /= b;
    a[2] /= b;
}

double check_intersect(const Vector &A, const Vector &B, const Vector &u, const Vector &v) {
    Vector N(v[1] - u[1], u[0] - v[0]);
    return dot(u - A, N) / dot(B - A, N);
}
double check_intersect_voronoi(const Vector &A, const Vector &B, const Vector &u, const Vector &v, double w1, double w2) {
    Vector M = (u + v) / 2 + (w1 - w2) / (2 * (v - u).norm2()) * (v - u);
    Vector N = u - v;

    return dot(M - A, N) / dot(B - A, N);
}

bool check_inside(const Vector &P, const Vector &u, const Vector&v) {
    Vector N = Vector(v[1] - u[1], u[0] - v[0]);
    return dot(P - u, N) <= 0;
}
bool check_inside_voronoi(const Vector &P, const Vector &u, const Vector &v, double w1, double w2) {
    Vector M = (u + v) / 2   + (w1 - w2) / (2 * (v - u).norm2()) * (v - u);
    return dot(P - M, u - v) > 0;
}