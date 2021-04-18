#ifndef TRANSFORMATOR_H
#define TRANSFORMATOR_H

#include <math.h>
#include <vector>

#include "point.h"

using std::vector;

class Transformator
{
public:
    Transformator();
    ~Transformator() {}

    void rotatePoint(Point& p, Point c, vector <double> r);
    void movePoint(Point& p, vector <int> m);
    void scalePoint(Point& p, Point c, vector <double> s);
};

#endif // TRANSFORMATOR_H
