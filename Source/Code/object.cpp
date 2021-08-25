#include "./Headers/object.h"

Object::Object(Point c, vector<int> d, QColor col)
{
    center = c;
    dimensions = d;
    color = col;
    inverted = false;

    formSides();
}

void Object::move(vector<int> d)
{
    center.x += d[0];
    center.y += d[1];
    center.z += d[2];

    collideCenter.x += d[0] / 10;
    collideCenter.y += d[1] / 10;
    collideCenter.z += d[2] / 10;

    for (size_t i = 0;i < sides.size(); ++i)
    {
        vector <Point>& temp = sides[i];

        for (size_t j = 0; j < temp.size(); ++j)
        {
            temp[j].x += d[0];
            temp[j].y += d[1];
            temp[j].z += d[2];
        }
    }

    for (size_t i = 0; i < connecters.size(); ++i)
        connecters[i].move(d);
}

void Object::setColor(QColor col)
{
    color = col;
    for (size_t i = 0; i < connecters.size(); ++i)
        connecters[i].setColor(col);
}

bool Object::isInside(Camera& view, Transformator& tr, pair <int, int> p)
{
    vector <vector <Point>> trSides;
    vector <vector <Point>> visibleSides;

    transformObject(view, tr, trSides);
    findVisibleSides(view.getPos(), trSides, visibleSides);
    vector <vector <pair <int, int>>> visSides = to2dSides(visibleSides);

    bool flag = false;

    for (size_t k = 0; k < visSides.size() && !flag; ++k)
    {
        vector <pair <int, int>> temp = visSides[k];
        for (size_t i = 1; i < temp.size(); ++i)
            {
                if ((((temp[i - 1].second <= p.second) && (p.second < temp[i].second)) || ((temp[i].second <= p.second) && (p.second < temp[i - 1].second))) &&
                  (p.first > (temp[i].first - temp[i - 1].first) * (p.second - temp[i - 1].second) / (temp[i].second - temp[i - 1].second) + temp[i - 1].first))
                    flag = !flag;
            }

        if ((((temp[temp.size() - 1].second <= p.second) && (p.second < temp[0].second)) || ((temp[0].second <= p.second) && (p.second < temp[temp.size() - 1].second))) &&
          (p.first > (temp[0].first - temp[temp.size() - 1].first) * (p.second - temp[temp.size() - 1].second) / (temp[0].second - temp[temp.size() - 1].second) + temp[temp.size() - 1].first))
            flag = !flag;
    }

    return flag;
}
