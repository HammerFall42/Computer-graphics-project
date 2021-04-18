#ifndef CAMERA_H
#define CAMERA_H

#include <math.h>
#include <vector>

#include "transformator.h"
#include "point.h"

using std::vector;

class Camera
{
public:
    Camera();
    ~Camera() {}

    void setPos(Point p) {pos = p;}
    void setCenter(Point c) {center = c;}
    void setRotated(vector <double> _r) {r = _r;}
    void setMoved(vector <int> _m) {m = _m;}
    void setScaled(vector <double> _s) {s = _s;}

    Point getPos() {return pos;}
    Point getCenter() {return center;}
    vector <double>& getRotated() {return r;}
    vector <int>& getMoved() {return m;}
    vector <double>& getScaled() {return s;}
    vector <double> getMinusRotated();
    vector <int> getMinusMoved();
    vector <double> getInvertScaled();

    void move(vector <int> _m);
    void scale(vector <double> _s);
    void rotate(vector <double> _r);

private:
    Point center;
    Point pos;
    vector <int> m;
    vector <double> r;
    vector <double> s;
};

#endif // CAMERA_H
