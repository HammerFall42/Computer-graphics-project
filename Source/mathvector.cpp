#include "mathvector.h"

MathVector::MathVector(Point l, Point r)
{
    x = r.x - l.x;
    y = r.y - l.y;
    z = r.z - l.z;
}

double MathVector::length()
{
    return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

void MathVector::normalize()
{
    double len = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
    x = x / len;
    y = y / len;
    z = z / len;
}
