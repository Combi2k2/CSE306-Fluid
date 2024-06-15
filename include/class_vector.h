#ifndef CLASS_VECTOR_H
#define CLASS_VECTOR_H

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
 
Vector operator + (const Vector &a, const Vector &b);
Vector operator - (const Vector &a, const Vector &b);
Vector operator * (const Vector &a, const Vector &b);
Vector operator * (const double &a, const Vector& b);
Vector operator * (const Vector &a, const double &b);
Vector operator / (const Vector &a, const double &b);

double dot(const Vector &a, const Vector &b);
Vector cross(const Vector &a, const Vector &b);

void operator += (Vector &a, const Vector &b);
void operator -= (Vector &a, const Vector &b);
void operator *= (Vector &a, const Vector &b);
void operator *= (Vector &a, const double &b);
void operator /= (Vector &a, const double &b);

double intersect(const Vector &A, const Vector &B, const Vector &u, const Vector &v);
double intersect_bisector(const Vector &A, const Vector &B, const Vector &u, const Vector &v, double w1, double w2);

bool inside(const Vector &P, const Vector &u, const Vector &v);
bool inside_bisector(const Vector &P, const Vector &u, const Vector &v, double w1, double w2);

#endif