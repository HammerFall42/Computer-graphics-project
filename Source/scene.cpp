#include "scene.h"

Scene::Scene()
{
    prevPos.first = -1;
    prevPos.second = -1;
    selected = -1;
    grouped = false;
    ctrlPressed = false;

    for (int i = 0; i < 100; ++i)
        for (int j = 0; j < 100; ++j)
            for (int k = 0; k < 100; ++k)
                collideBox[i][j][k] = false;

    Point c;
    vector <int> d;
    QColor col = QColor("red");

    int height[2] = {4, 2};
    int width[2] = {4, 2};
    int length[3] = {6, 4, 2};

    d.push_back(0);
    d.push_back(0);
    d.push_back(0);

    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 2; ++j)
            for (int k = 0; k < 3; ++k)
            {
                int mod = Object::getModifier();

                d[0] = width[j];
                d[1] = height[i];
                d[2] = length[k];

                c.x = mod * d[0] / 2 + 30;
                c.y = mod * d[1] / 2 + 55;
                c.z = mod * d[2] / 2;

                Object obj(c, d, col);

                for (int u = 0; u < width[j] / 2; ++u)
                    for (int w = 0; w < length[k] / 2; ++w)
                    {
                        Point p(c.x - (mod * d[0] / 2) + (2 * u + 1) * mod,
                                c.y - (mod * d[1] / 2 + 5),
                                c.z - (mod * d[2] / 2) + (2 * w + 1) * mod);
                        vector <int> vec{1, 1, 1};
                        obj.addConnecter(Connecter(p, vec, col));
                    }

                shapes.push_back(obj);
            }

    d[0] = 100;
    d[1] = 100;
    d[2] = 100;

    c.x = Object::getModifier() * d[0] / 2;
    c.y = Object::getModifier() * d[1] / 2;
    c.z = Object::getModifier() * d[2] / 2;

    Object o(Object(c, d, QColor(255,118,185)));
    o.invert();

    objects.push_back(o);
}

vector <Point> Scene::getCenters()
{
    vector <Point> centers;

    for (size_t i = 0; i < objects.size(); ++i)
    {
        centers.push_back(objects[i].getCenter());
        vector <Connecter> cons = objects[i].getConnecters();
        for (size_t j = 0; j < cons.size(); ++j)
            centers.push_back(cons[j].getCenter());
    }

    return centers;
}

vector <BaseObject> Scene::getObjects()
{
    vector <BaseObject> objs;

    for (size_t i = 0; i < objects.size(); ++i)
    {
        objs.push_back(BaseObject(objects[i].getCenter(), objects[i].getDimensions(), objects[i].getColor()));

        if (objects[i].isInverted()) objs[i].invert();

        vector <Connecter> cons = objects[i].getConnecters();
        for (size_t j = 0; j < cons.size(); ++j)
            objs.push_back(BaseObject(cons[j].getCenter(), cons[j].getDimensions(), cons[j].getColor()));
    }

    return objs;
}

vector <BaseObject> Scene::getPanelObjects()
{
    vector <BaseObject> objs;

    for (size_t i = 0; i < shapes.size(); ++i)
    {
        objs.push_back(BaseObject(shapes[i].getCenter(), shapes[i].getDimensions(), shapes[i].getColor()));

        vector <Connecter> cons = shapes[i].getConnecters();
        for (size_t j = 0; j < cons.size(); ++j)
            objs.push_back(BaseObject(cons[j].getCenter(), cons[j].getDimensions(), cons[j].getColor()));
    }

    return objs;
}

void Scene::addObject(QColor col)
{
    Object obj = shapes[panelIndex];
    vector <int> d = {-30, -40, 0};
    vector <int> m = {0, 0, 0};
    Point colCenter;
    Point center;
    vector <int> dim = obj.getDimensions();

    checkSpawnCollision(m);
    for (size_t i = 0; i < d.size(); ++i)
        d[i] += m[i] * 10;

    obj.move(d);
    center = obj.getCenter();

    colCenter[0] = center[0] / 10 - dim[0] / 2;
    colCenter[1] = center[1] / 10 - dim[1] / 2;
    colCenter[2] = center[2] / 10 - dim[2] / 2;

    obj.setCollideCenter(colCenter);

    for (int i = 0; i < dim[0]; ++i)
        for (int j = 0; j < dim[1]; ++j)
            for (int k = 0; k < dim[2]; ++k)
                collideBox[colCenter[0] + i][colCenter[1] + j][colCenter[2] + k] = true;


    obj.setColor(col);
    objects.push_back(obj);
    selected = objects.size() - 1;
}

void Scene::removeObject()
{
    int sz = objects.size();

    if (grouped)
    {
        vector <Object> temp;
        for (size_t i = 0; i < objects.size(); ++i)
        {
            if (std::find(group.begin(), group.end(), i) != group.end())
                continue;
            temp.push_back(objects[i]);
        }

        objects = temp;
        selected = -1;
        grouped = false;
        group.clear();

        return;
    }

    if (selected <= 0 || selected > (sz - 1)) return;

    objects.erase(objects.begin() + selected);
}


void Scene::findSelected(Sorter &s, Camera &cam, Transformator &tr, pair<int, int> p)
{
    group.clear();
    grouped = false;

    vector <Point> centers;
    vector <pair <double, int>> c;

    for (size_t i = 0; i < objects.size(); ++i)
        centers.push_back(objects[i].getCenter());

    for (size_t i = 0; i < centers.size(); ++i)
    {
        pair <double, int> temp;
        double len = MathVector(cam.getPos(), centers[i]).length();
        temp.first = len;
        temp.second = i;
        c.push_back(temp);
    }
    s.Sort(c);

    for (int i = c.size() - 1; i >= 0; --i)
    {
        if (objects[c[i].second].isInside(cam, tr, p))
        {
            selected = c[i].second;
            return;
        }
    }

    selected = -1;
}

void Scene::findGroupSelected(Sorter &s, Camera &cam, Transformator &tr, pair<int, int> p)
{
    vector <Point> centers;
    vector <pair <double, int>> c;

    for (size_t i = 0; i < objects.size(); ++i)
        centers.push_back(objects[i].getCenter());

    for (size_t i = 0; i < centers.size(); ++i)
    {
        pair <double, int> temp;
        double len = MathVector(cam.getPos(), centers[i]).length();
        temp.first = len;
        temp.second = i;
        c.push_back(temp);
    }
    s.Sort(c);

    for (int i = c.size() - 1; i >= 0; --i)
    {
        if (c[i].second != 0 && objects[c[i].second].isInside(cam, tr, p))
        {
            if (std::find(group.begin(), group.end(), c[i].second) != group.end())
                continue;
            group.push_back(c[i].second);
            grouped = true;
            return;
        }
    }
}

void Scene::keyPressEvent(QKeyEvent *event)
{
    vector <int> m = {0, 0, 0};

    if (event->key() == Qt::Key_Control)
        ctrlPressed = true;

    if (event->key() == Qt::Key_Up)
    {
        m[1] = -100;
        emit moveCam(m);
    }
    else if (event->key() == Qt::Key_Left)
    {
        m[0] = -100;
        emit moveCam(m);
    }
    else if (event->key() == Qt::Key_Down)
    {
        m[1] = 100;
        emit moveCam(m);
    }
    else if (event->key() == Qt::Key_Right)
    {
        m[0] = 100;
        emit moveCam(m);
    }
    else if (event->key() == Qt::Key_Q)
    {
        m[0] = 10;

        if (grouped)
            moveGroup(m);
        else
            moveObject(m, true);
    }
    else if (event->key() == Qt::Key_A)
    {
        m[0] = -10;

        if (grouped)
            moveGroup(m);
        else
            moveObject(m, true);
    }
    else if (event->key() == Qt::Key_W)
    {
        m[1] = 10;

        if (grouped)
            moveGroup(m);
        else
            moveObject(m, true);
    }
    else if (event->key() == Qt::Key_S)
    {
        m[1] = -10;

        if (grouped)
            moveGroup(m);
        else
            moveObject(m, true);
    }
    else if (event->key() == Qt::Key_E)
    {
        m[2] = 10;

        if (grouped)
            moveGroup(m);
        else
            moveObject(m, true);
    }
    else if (event->key() == Qt::Key_D)
    {
        m[2] = -10;

        if (grouped)
            moveGroup(m);
        else
            moveObject(m, true);
    }
}

void Scene::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control)
        ctrlPressed = false;
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
        return;

    pair <int, int> p;
    p.first = event->scenePos().x();
    p.second = event->scenePos().y();


    if (ctrlPressed)
        emit checkGroupInside(p);
    else
        emit checkInside(p);
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event)

    prevPos.first = -1;
    prevPos.second = -1;
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->buttons().testFlag(Qt::LeftButton))
        return;

    if (prevPos.first == -1 && prevPos.second == -1)
    {
        prevPos.first = event->scenePos().x();
        prevPos.second = event->scenePos().y();
        return;
    }

    vector <double> r = {0, 0, 0};
    int x = event->screenPos().x() - prevPos.first;
    int y = event->screenPos().y() - prevPos.second;

    if (abs(x) >= abs(y))
    {
        if (x > 0)
            r[1] = 0.05;
        else
            r[1] = -0.05;
    }
    else
    {
        if (y > 0)
            r[0] = 0.05;
        else
            r[0] = -0.05;
    }

    prevPos.first = event->screenPos().x();
    prevPos.second = event->screenPos().y();

    emit rotateCam(r);
}

void Scene::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    vector <double> s = {0, 0, 0};

    if (event->delta() > 0)
    {
        s[0] = 1.1;
        s[1] = 1.1;
        s[2] = 1.1;
    }
    else
    {
        s[0] = 0.9;
        s[1] = 0.9;
        s[2] = 0.9;
    }

    emit scaleCam(s);
}

void Scene::moveGroup(vector<int> m)
{
    vector <Object> test;
    vector <int> moved;
    vector <int> invM{-m[0], -m[1], -m[2]};

    for (size_t i = 0; i < group.size(); ++i)
        test.push_back(objects[group[i]]);

    if (m[0] > 0) Sorter::xSort(test, group, true);
    else if (m[0] < 0) Sorter::xSort(test, group, false);
    else if (m[1] > 0) Sorter::ySort(test, group, true);
    else if (m[1] < 0) Sorter::ySort(test, group, false);
    else if (m[2] > 0) Sorter::zSort(test, group, true);
    else if (m[2] < 0) Sorter::zSort(test, group, false);

    for (size_t i = 0; i < test.size(); ++i)
    {
        selected = group[i];
        if (!checkCollision(m))
        {
            moveObject(m, false);
            moved.push_back(selected);
        }
        else
        {
            for (size_t j = 0; j < moved.size(); ++j)
                moveObject(invM, false);
            return;
        }
    }
}

void Scene::moveObject(vector<int> m, bool check)
{
    if (selected <= 0) return;

    if (check && checkCollision(m))
        return;

    vector <int> d = {0, 0, 0};
    vector <int> dim = objects[selected].getDimensions();
    Point colCenter = objects[selected].getCollideCenter();
    objects[selected].move(m);

    if (m[0])
    {
        if (m[0] > 0)
            for (int i = 0; i < dim[1]; ++i)
                for (int j = 0; j < dim[2]; ++j)
                {
                    collideBox[colCenter[0] + dim[0]][colCenter[1] + i][colCenter[2] + j] = \
                            !collideBox[colCenter[0] + dim[0]][colCenter[1] + i][colCenter[2] + j];

                    collideBox[colCenter[0]][colCenter[1] + i][colCenter[2] + j] = \
                            !collideBox[colCenter[0]][colCenter[1] + i][colCenter[2] + j];
                }
        else
            for (int i = 0; i < dim[1]; ++i)
                for (int j = 0; j < dim[2]; ++j)
                {
                    collideBox[colCenter[0] - 1][colCenter[1] + i][colCenter[2] + j] = \
                            !collideBox[colCenter[0] - 1][colCenter[1] + i][colCenter[2] + j];

                    collideBox[colCenter[0] + dim[0] - 1][colCenter[1] + i][colCenter[2] + j] = \
                            !collideBox[colCenter[0] + dim[0] - 1][colCenter[1] + i][colCenter[2] + j];
                }
    }
    else if (m[1])
    {
        if (m[1] > 0)
            for (int i = 0; i < dim[0]; ++i)
                for (int j = 0; j < dim[2]; ++j)
                {
                    collideBox[colCenter[0] + i][colCenter[1] + dim[1]][colCenter[2] + j] = \
                            !collideBox[colCenter[0] + i][colCenter[1] + dim[1]][colCenter[2] + j];

                    collideBox[colCenter[0] + i][colCenter[1]][colCenter[2] + j] = \
                            !collideBox[colCenter[0] + i][colCenter[1]][colCenter[2] + j];
                }
        else
        {
            for (int i = 0; i < dim[0]; ++i)
                for (int j = 0; j < dim[2]; ++j)
                {
                    collideBox[colCenter[0] + i][colCenter[1] + dim[1] - 1][colCenter[2] + j] = \
                            !collideBox[colCenter[0] + i][colCenter[1] + dim[1] - 1][colCenter[2] + j];

                    collideBox[colCenter[0] + i][colCenter[1] - 1][colCenter[2] + j] = \
                            !collideBox[colCenter[0] + i][colCenter[1] - 1][colCenter[2] + j];
                }
        }
    }
    else
    {
        if (m[2] > 0)
            for (int i = 0; i < dim[0]; ++i)
                for (int j = 0; j < dim[1]; ++j)
                {
                    collideBox[colCenter[0] + i][colCenter[1] + j][colCenter[2] + dim[2]] = \
                            !collideBox[colCenter[0] + i][colCenter[1] + j][colCenter[2] + dim[2]];

                    collideBox[colCenter[0] + i][colCenter[1] + j][colCenter[2]] = \
                            !collideBox[colCenter[0] + i][colCenter[1] + j][colCenter[2]];
                }
        else
        {
            for (int i = 0; i < dim[0]; ++i)
                for (int j = 0; j < dim[1]; ++j)
                {
                    collideBox[colCenter[0] + i][colCenter[1] + j][colCenter[2] + dim[2] - 1] = \
                            !collideBox[colCenter[0] + i][colCenter[1] + j][colCenter[2] + dim[2] - 1];

                    collideBox[colCenter[0] + i][colCenter[1] + j][colCenter[2] - 1] = \
                            !collideBox[colCenter[0] + i][colCenter[1] + j][colCenter[2] - 1];
                }
        }
    }

    emit moveCam(d);
}

bool Scene::checkCollision(vector <int> m)
{
    bool flag = false;

    if (m[0]) flag = checkColOX(m[0]);
    else if (m[1]) flag = checkColOY(m[1]);
    else if (m[2]) flag = checkColOZ(m[2]);

    return flag;
}

bool Scene::checkColOX(int d)
{
    bool flag = false;
    int md, dt;
    vector <int> dim = objects[selected].getDimensions();
    Point colCenter = objects[selected].getCollideCenter();

    if (d > 0)
    {
        md = 1;
        dt = 0;
    }
    else
    {
        md = 0;
        dt = -1;
    }

    for (int i = 0; i < dim[1] && !flag; ++i)
        for (int j = 0; j < dim[2] && !flag; ++j)
            if (collideBox[colCenter[0] + md * dim[0] + dt][colCenter[1] + i][colCenter[2] + j])
                flag  = true;

    return flag;
}

bool Scene::checkColOY(int d)
{
    bool flag = false;
    int md, dt;
    vector <int> dim = objects[selected].getDimensions();
    Point colCenter = objects[selected].getCollideCenter();

    if (d > 0)
    {
        md = 1;
        dt = 0;
    }
    else
    {
        md = 0;
        dt = -1;
    }

    for (int i = 0; i < dim[0] && !flag; ++i)
        for (int j = 0; j < dim[2] && !flag; ++j)
            if (collideBox[colCenter[0] + i][colCenter[1] + md * dim[1] + dt][colCenter[2] + j])
                flag  = true;

    return flag;
}

bool Scene::checkColOZ(int d)
{
    bool flag = false;
    int md, dt;
    vector <int> dim = objects[selected].getDimensions();
    Point colCenter = objects[selected].getCollideCenter();

    if (d > 0)
    {
        md = 1;
        dt = 0;
    }
    else
    {
        md = 0;
        dt = -1;
    }

    for (int i = 0; i < dim[0] && !flag; ++i)
        for (int j = 0; j < dim[1] && !flag; ++j)
            if (collideBox[colCenter[0] + i][colCenter[1] + j][colCenter[2] + md * dim[2] + dt])
                flag  = true;

    return flag;
}

bool Scene::checkSpawnCollision(vector <int>& d)
{
    Point c = Point(0, 0, 0);
    vector <int> dim = shapes[panelIndex].getDimensions();
    bool flag = false;

    while (!flag)
    {
        for (int i = 0; i < dim[0] && !flag; ++i)
            for (int j = 0; j < dim[1] && !flag; ++j)
                for (int k = 0; k < dim[2] && !flag; ++k)
                    if (collideBox[c[0] + i][c[1] + j][c[2] + k])
                        flag  = true;

        if (!flag) break;
        else flag = false;

        if (c[0] + dim[0] < 100)
            c[0] += dim[0];
        else if (c[1] + dim[1] < 100)
        {
            c[0] = 0;
            c[1] += dim[1];
        }
        else if (c[2] + dim[2] < 100)
        {
            c[0] = 0;
            c[1] = 0;
            c[2] += dim[2];
        }
        else flag = true;
    }

    if (flag) return flag;

    for (size_t i = 0; i < 3; ++i)
        d[i] += c[i];

    return flag;
}
