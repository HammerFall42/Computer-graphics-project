#include "./Headers/transformator.h"

Transformator::Transformator() {}

void Transformator::rotatePoint(Point &p, Point c, vector<double> r)
{
    double y_copy = p.y;
    p.y = c.y + (p.y - c.y) * cos(r[0]) + (p.z - c.z) * sin(r[0]);
    p.z = c.z + (c.y - y_copy) * sin(r[0]) + (p.z - c.z) * cos(r[0]);

    double x_copy = p.x;
    p.x = c.x + (p.x - c.x) * cos(r[1]) + (p.z - c.z) * sin(r[1]);
    p.z = c.z + (c.x - x_copy) * sin(r[1]) + (p.z - c.z) * cos(r[1]);

    x_copy = p.x;
    p.x = c.x + (p.x - c.x) * cos(r[2]) - (p.y - c.y) * sin(r[2]);
    p.y = c.y + (x_copy - c.x) * sin(r[2]) + (p.y - c.y) * cos(r[2]);
}

void Transformator::movePoint(Point &p, vector<int> m)
{
    p.x += m[0];
    p.y += m[1];
    p.z += m[2];
}

void Transformator::scalePoint(Point &p, Point c, vector<double> s)
{
    p.x = c.x + s[0] * (p.x - c.x);
    p.y = c.y + s[1] * (p.y - c.y);
    p.z = c.z + s[2] * (p.z - c.z);
}
