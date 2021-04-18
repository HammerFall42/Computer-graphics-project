#include "camera.h"

Camera::Camera()
{
    center = Point(500, 500, 500);

    pos = Point(500, 500, -10000);

    m.push_back(0);
    m.push_back(-100);
    m.push_back(0);

    s.push_back(0.6);
    s.push_back(0.6);
    s.push_back(0.6);

    r.push_back(-0.1);
    r.push_back(-0.75);
    r.push_back(0);
}

void Camera::move(vector<int> _m)
{
    for (int i = 0; i < 3; ++i)
        m[i] -= _m[i];
}

void Camera::rotate(vector<double> _r)
{
    for (int i = 0; i < 3; ++i)
        r[i] -= _r[i];
}

void Camera::scale(vector<double> _s)
{
    for (int i = 0; i < 3; ++i)
        s[i] *= _s[i];
}

vector <int> Camera::getMinusMoved()
{
    vector <int> temp = m;

    for (size_t i = 0; i < temp.size(); ++i)
        temp[i] *= -1;

    return temp;
}

vector <double> Camera::getMinusRotated()
{
    vector <double> temp = r;

    for (size_t i = 0; i < temp.size(); ++i)
        temp[i] *= -1;

    return temp;
}

vector <double> Camera::getInvertScaled()
{
    vector <double> temp = s;

    for (size_t i = 0; i < temp.size(); ++i)
        temp[i] = 1 / temp[i];

    return temp;
}
