#ifndef FACADE_H
#define FACADE_H

#include "transformator.h"
#include "camera.h"
#include "painter.h"

class Facade
{
public:
    Facade();
    ~Facade() {}

    Camera& getCam() {return cam;}

    void setScene(Scene* sc) {painter.setScene(sc);}
    void setPanel(Panel **pn) {painter.setPanel(pn);}
    void setColor(QColor col) {painter.setFC(col);}

    void moveCam(vector <int>& m);
    void rotateCam(vector <double>& r);
    void scaleCam(vector <double>& s);
    void initCam();

    void paintPanel();
    void paintScene();

private:
    Camera cam;
    Transformator trans;
    Painter painter;
};

#endif // FACADE_H
