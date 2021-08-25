#include "./Headers/connecter.h"

Connecter::Connecter(Point c, vector<int> d, QColor col)
{
    center = c;
    dimensions = d;
    color = col;
}

void Connecter::move(vector<int> d)
{
    center.x += d[0];
    center.y += d[1];
    center.z += d[2];

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
}
