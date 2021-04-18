#ifndef MATHVECTOR_H
#define MATHVECTOR_H

#include <math.h>

#include "point.h"

class MathVector
{
public:
    MathVector() : x(0), y(0), z(0) {}
    MathVector(const double _x, const double _y, const double _z) : x(_x), y(_y), z(_z) {}
    MathVector(Point l, Point r);
    ~MathVector() {}

    friend MathVector operator*(const MathVector& l, const MathVector& r)
    {
        int x = l.getY() * r.getZ() - l.getZ() * r.getY();
        int y = l.getZ() * r.getX() - l.getX() * r.getZ();
        int z = l.getX() * r.getY() - l.getY() * r.getX();

        return MathVector(x, y, z);
    }

    friend double operator&(const MathVector& l, const MathVector& r)
    {
       return l.x * r.x + l.y * r.y + l.z * r.z;
    }

    friend double operator^(MathVector& l, MathVector& r)
    {
        return acos((l & r) / (l.length() * r.length()));
    }


    double length();
    void normalize();

    double getX() const {return x;}
    double getY() const {return y;}
    double getZ() const {return z;}

    void setX(const double _x) {x = _x;}
    void setY(const double _y) {y = _y;}
    void setZ(const double _z) {z = _z;}

private:
    double x, y, z;
};

#endif // MATHVECTOR_H
