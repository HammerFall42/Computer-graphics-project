#ifndef POINT_H
#define POINT_H

#include <math.h>
#include <vector>
#include <map>

using namespace std;

class Point
{
public:
    Point() {}
    Point(double _x, double _y, double _z);
    ~Point() {}

    pair <int, int> to2d();

    int operator==(const Point&p);
    Point& operator=(const Point& p);
    int& operator[](const int i);

    int x, y, z;
};

#endif // POINT_H
