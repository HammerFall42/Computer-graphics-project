#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QObject>

#include "object.h"
#include "sorter.h"

class Scene : public QGraphicsScene
{
    Q_OBJECT

public:
    Scene();
    ~Scene() {}

    void setPanelIndex(int ind) {panelIndex = ind;}

    vector <Point> getCenters();
    vector <BaseObject> getObjects();
    vector <BaseObject> getPanelObjects();
    vector <Object> getCore() {return objects;}
    vector <Object> getShapes() {return shapes;}

    void findSelected(Sorter& s, Camera& cam, Transformator& tr, pair <int, int> p);
    void findGroupSelected(Sorter& s, Camera& cam, Transformator& tr, pair <int, int> p);
    void addObject(QColor col);
    void removeObject();

signals:
    void moveCam(vector <int> m);
    void rotateCam(vector <double> r);
    void scaleCam(vector <double> s);

    void checkInside(pair <int, int> p);
    void checkGroupInside(pair <int, int> p);

private:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void wheelEvent(QGraphicsSceneWheelEvent *event) Q_DECL_OVERRIDE;
    void moveObject(vector <int> m, bool check);
    void moveGroup(vector <int> m);
    bool checkCollision(vector <int> m);
    bool checkSpawnCollision(vector<int> &d);
    bool checkColOX(int d);
    bool checkColOY(int d);
    bool checkColOZ(int d);

    int selected;
    int panelIndex;
    pair <int, int> prevPos;
    vector <Object> objects;
    vector <Object> shapes;
    vector <int> group;
    bool collideBox[100][100][100];
    bool grouped;
    bool ctrlPressed;
};

#endif // SCENE_H
