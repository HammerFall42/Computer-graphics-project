#include "facade.h"

Facade::Facade() {}

void Facade::paintPanel()
{
    painter.setLastRepainted(-1);
    painter.repaintPanel(-1, QColor("white"));
}

void Facade::paintScene()
{
    painter.repaintScene(cam);
}

void Facade::moveCam(vector<int> &m)
{
    cam.move(m);
    painter.repaintScene(cam);
}

void Facade::rotateCam(vector<double> &r)
{
    cam.rotate(r);
    painter.repaintScene(cam);
}

void Facade::scaleCam(vector<double> &s)
{
    cam.scale(s);
    painter.repaintScene(cam);
}

void Facade::initCam()
{
    cam = Camera();
    painter.repaintScene(cam);
}
