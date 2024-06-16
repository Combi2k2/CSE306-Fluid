#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>

class Vector {
private:
    double data[3];
public:
    explicit Vector(double x = 0, double y = 0, double z = 0) {
        data[0] = x;
        data[1] = y;
        data[2] = z;
    }
    inline double norm2() const {
        return data[0] * data[0] + data[1] * data[1] + data[2] * data[2];
    }
    inline double norm() const {
        return sqrt(norm2());
    }
    inline void normalize() {
        double n = norm();
        data[0] /= n;
        data[1] /= n;
        data[2] /= n;
    }
    inline double  operator[](int i) const {
        return data[i];
    }
    inline double& operator[](int i) {
        return data[i];
    };
};
 
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

#endif