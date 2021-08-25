#include "./Headers/baseobject.h"

BaseObject::BaseObject(Point c, vector<int> d, QColor col)
{
    center = c;
    dimensions = d;
    color = col;
    inverted = false;

    formSides();
}

void BaseObject::formSides()
{
    vector <Point> side;
    Point temp;
    int signs[6][12] = {
        {1, 1, 1, -1, 1, 1, -1, 1, -1, 1, 1, -1},
        {1, -1, 1, 1, -1, -1, -1, -1, -1, -1, -1, 1},
        {1, 1, 1, 1, -1, 1, -1, -1, 1, -1, 1, 1},
        {1, 1, -1, -1, 1, -1, -1, -1, -1, 1, -1, -1},
        {1, 1, 1, 1, 1, -1, 1, -1, -1, 1, -1, 1},
        {-1, 1, 1, -1, -1, 1, -1, -1, -1, -1, 1, -1}
        };

    double d0 = dimensions[0] * modifier / 2;
    double d1 = dimensions[1] * modifier / 2;
    double d2 = dimensions[2] * modifier / 2;

    for (int i = 0; i < 6; ++ i)
    {
        temp.x = center.x + signs[i][0] * d0;
        temp.y = center.y + signs[i][1] * d1;
        temp.z = center.z + signs[i][2] * d2;
        side.push_back(temp);

        temp.x = center.x + signs[i][3] * d0;
        temp.y = center.y + signs[i][4] * d1;
        temp.z = center.z + signs[i][5] * d2;
        side.push_back(temp);

        temp.x = center.x + signs[i][6] * d0;
        temp.y = center.y + signs[i][7] * d1;
        temp.z = center.z + signs[i][8] * d2;
        side.push_back(temp);

        temp.x = center.x + signs[i][9] * d0;
        temp.y = center.y + signs[i][10] * d1;
        temp.z = center.z + signs[i][11] * d2;
        side.push_back(temp);

        sides.push_back(side);
        side.clear();
    }
}

void BaseObject::findVisibleSides(Point view, vector <vector <Point>>& trSides, vector <vector <Point>>& visibleSides)
{
    for (size_t i = 0; i < trSides.size(); ++i)
    {
        vector <Point> side = trSides[i];
        MathVector v1 = MathVector(side[1], side[2]);
        MathVector v2 = MathVector(side[1], side[0]);
        MathVector n = v1 * v2;
        MathVector v = MathVector(view, side[1]);

        v.normalize();
        n.normalize();

        double comp = v & n;
        /*
        double angle = acos((v & n) / (v.length() * n.length()));
        forDebug += "Угол " + QString::number(angle) + "\nКосинус " + QString::number(cos(angle)) + " \n";
        */
        if (inverted)
            comp *= -1;
        if (comp >= 0)
            visibleSides.push_back(side);
    }
}

void BaseObject::transformObject(Camera &cam, Transformator& tr, vector <vector <Point>>& trSides)
{
    for (size_t i = 0; i < sides.size(); ++i)
    {
        vector <Point> side = sides[i];
        for (size_t j = 0; j < side.size(); ++j)
        {
            tr.movePoint(side[j], cam.getMoved());
            tr.rotatePoint(side[j], cam.getCenter(), cam.getRotated());
            tr.scalePoint(side[j], cam.getCenter(), cam.getScaled());
        }
        trSides.push_back(side);
    }
}

vector <vector <pair <int, int>>> BaseObject::to2dSides(vector <vector <Point>>& visibleSides)
{
    vector <vector <pair <int, int>>> tr2dSides;
    for (size_t i = 0; i < visibleSides.size(); ++i)
    {
        vector <pair <int, int>> side;
        for (size_t j = 0; j < visibleSides[i].size(); ++j)
        {
            Point tmp = visibleSides[i][j];
            side.push_back(tmp.to2d());
        }
        tr2dSides.push_back(side);
        side.clear();
    }

    return tr2dSides;
}

vector<vector<Point> > BaseObject::getVisibleSides(Camera& cam, Transformator& tr)
{
    vector <vector <Point>> trSides;
    vector <vector <Point>> visibleSides;

    transformObject(cam, tr, trSides);

    findVisibleSides(cam.getPos(), trSides, visibleSides);

    return visibleSides;
}
