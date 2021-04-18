#ifndef PAINTER_H
#define PAINTER_H

#include <QColor>
#include <thread>

#include "camera.h"
#include "scene.h"
#include "panel.h"

using namespace std;

class Painter : QObject
{
    Q_OBJECT

public:
    Painter();
    ~Painter();

    void setFC(QColor col) {FC = col;}
    void setLC(QColor col) {LC = col;}
    void setScene(Scene *sc) {scene = sc;}
    void setPanel(Panel **pn);
    void setTransformator(Transformator& t) {trans = t;}
    void setLastRepainted(int num) {lastPanelRepainted = num;}

    void repaintScene(Camera& cam);
    void repaintPanel(int ind, QColor col);

private slots:
    void onPanelClicked(int num);

private:
    void putToZBuffer(vector<Point> side, QColor col, MathVector h);
    void paintSides(vector <vector <Point>> &visSides, Point pos, QColor col);
    void paintEdges(vector <vector <Point>> sides, Point pos, QColor col);
    void paintPanelElement(BaseObject obj);
    void fillBuffer(size_t lower, size_t upper, QImage &img);
    void panelZBuf(vector<Point> side, QColor col);

    int **screen;
    QColor **buffer;
    int **panelScr;
    QColor **panelBuf;
    Point light;
    QColor FC, LC;
    Scene *scene;
    Panel **panel;
    Transformator trans;
    int lastPanelRepainted;
};

#endif // PAINTER_H
