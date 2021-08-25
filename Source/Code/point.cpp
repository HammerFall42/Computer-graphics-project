#include "./Headers/point.h"

Point::Point(double _x, double _y, double _z)
{
    x = _x;
    y = _y;
    z = _z;
}

pair<int, int> Point::to2d()
{
    pair <int, int> p;

    p.first = x;
    p.second = y;
    /*p.first = k * x / (z + k);
    p.second = k * y / (z + k);*/

    return p;
}

int Point::operator==(const Point&p)
{
    if (x == p.x && y == p.y && z == p.z)
        return 1;
    else return 0;
}

Point& Point::operator=(const Point& p)
{
    x = p.x;
    y = p.y;
    z = p.z;

    return *this;
}

int& Point::operator[](const int i)
{
    if (i == 0) return x;
    else if (i == 1) return y;
    else if (i == 2) return z;

    return x;
}
