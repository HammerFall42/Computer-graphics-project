#ifndef BASEOBJECT_H
#define BASEOBJECT_H

#include <QColor>
#include <vector>
#include <math.h>

#include "camera.h"
#include "mathvector.h"
#include "point.h"

class BaseObject
{
public:
    BaseObject() {}
    BaseObject(Point c, vector<int> d, QColor col);
    ~BaseObject() {}

    Point getCenter() {return center;}
    vector <vector <Point>> getVisibleSides(Camera &cam, Transformator &tr);
    vector <vector <Point>> getSides() {return sides;}
    vector <int> getDimensions() {return dimensions;}
    QColor getColor() {return color;}

    void setColor(QColor col) {color = col;}
    void invert() {inverted = !inverted;}
    bool isInverted() {return inverted;}

protected:
    void formSides();
    void findVisibleSides(Point view, vector<vector<Point> > &trSides, vector<vector<Point> > &visibleSides);
    void transformObject(Camera& cam, Transformator &tr, vector<vector<Point> > &trSides);
    vector <vector <pair <int, int>>> to2dSides(vector <vector <Point>>& visibleSides);

    static const int modifier = 10;
    Point center;
    vector <int> dimensions;
    vector <vector <Point>> sides;
    QColor color;
    bool inverted;
};

#endif // BASEOBJECT_H
