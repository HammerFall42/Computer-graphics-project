#include "./Headers/painter.h"

Painter::Painter()
{
    LC = QColor("black");

    light = Point(500, 1000, 500);

    screen = new int*[2000];
    buffer = new QColor*[2000];
    panelScr = new int*[120];
    panelBuf = new QColor*[120];

    for (int i = 0; i < 2000; ++i)
    {
        screen[i] = new int[2000];
        buffer[i] = new QColor[2000];
    }

    for (int i = 0; i < 120; ++i)
    {
        panelScr[i] = new int[120];
        panelBuf[i] = new QColor[120];
    }

    lastPanelRepainted = -1;
}

Painter::~Painter()
{
    for (int i = 0; i < 2000; ++i)
    {
        delete screen[i];
        delete buffer[i];
    }
    delete screen;
    delete buffer;

    for (int i = 0; i < 120; ++i)
    {
        delete panelScr[i];
        delete panelBuf[i];
    }
    delete panelScr;
    delete panelBuf;
}

void Painter::setPanel(Panel **pn)
{
    panel = pn;
    for (int i = 0;i < 12; ++i)
        QObject::connect(panel[i], SIGNAL(shapeIsChosen(int)), this, SLOT(onPanelClicked(int)));
}


void Painter::putToZBuffer(vector<Point> side, QColor col, MathVector h)
{
    MathVector n = MathVector(side[1], side[2]) * MathVector(side[1], side[0]);
    MathVector l(side[0], light);

    int r, g, b;
    col.getRgb(&r, &g, &b);

    n.normalize();
    l.normalize();
    h.normalize();

    double K1, K2;

    K1 = n & l;
    K2 = n & h;

    while (abs(K1) < 100 && abs(K2) < 100)
    {
        K1 *= 10;
        K2 *= 10;
    }

    K1 /= 10;
    K2 /= 10;

    if (r)
        r = (int)(0.8 * r + 0.8 * K1 + 0.65 * K2);
    if (g)
        g = (int)(0.8 * g + 0.8 * K1 + 0.65 * K2);
    if (b)
        b = (int)(0.8 * b + 0.8 * K1 + 0.65 * K2);

    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;

    if (r < 0) r = 0;
    if (g < 0) g = 0;
    if (b < 0) b = 0;

    QColor c(r, g, b);

    int size = side.size();
    int ymax, ymin, ysc, e1, e, i;
    int x[size], y[size];

    for (i = 0; i < size; i++)
        x[i] = int(side[i].x), y[i] = int(side[i].y);

    ymax = ymin = y[0];

    for (int i = 1; i < size; ++i)
    {
        ymax = std::max(ymax, y[i]);
        ymin = std::min(ymin, y[i]);
    }

    ymin = (ymin < 0) ? 0 : ymin;
    ymax = (ymax < 2000) ? ymax : 2000;

    int ne;
    int x1, x2, xsc1, xsc2;
    double z1, z2, tc, z;

    for (ysc = ymin; ysc < ymax; ysc++)
    {
        ne = 0;
        for (int e = 0; e < size; e++)
        {
            e1 = e + 1;
            if (e1 == size) e1 = 0;
            if (y[e] < y[e1])
            {
                if (y[e1] <= ysc || ysc < y[e]) continue;
            }
            else if (y[e] > y[e1])
            {
                if (y[e1] > ysc || ysc >= y[e]) continue;
            }
            else continue;

            tc = double(y[e] - ysc) / (y[e] - y[e1]);
            if (ne)
            {
                x2 = x[e] + int (tc * (x[e1] - x[e]));
                z2 = side[e].z + tc * (side[e1].z - side[e].z);
            }
            else
            {
                x1 = x[e] + int (tc * (x[e1] - x[e]));
                z1 = side[e].z + tc * (side[e1].z - side[e].z);
                ne = 1;
            }
        }

        if (x2 < x1)
        {
            e = x1;
            x1 = x2;
            x2 = e;
            tc = z1;
            z1 = z2;
            z2 = tc;
        }

        xsc1 = (x1 < 0) ? 0 : x1;
        xsc2 = (x2 < 2000) ? x2 : 2000;

        for (int xsc = xsc1; xsc < xsc2; xsc++)
        {
            tc = double(x1 - xsc) / (x1 - x2);
            z = z1 + tc * (z2 - z1);

            if (z < screen[xsc][ysc])
            {
                buffer[xsc][ysc] = c;
                screen[xsc][ysc] = z;
            }
        }
    }
}

void Painter::paintSides(vector<vector<Point> > &visSides, Point pos, QColor col)
{
    for (size_t i = 0; i < visSides.size(); ++i)
    {
        vector <Point> temp = visSides[i];
        putToZBuffer(temp, col, MathVector(temp[0], pos));
        paintEdges(visSides, pos, col);
    }
}

void Painter::paintEdges(vector<vector<Point> > sides, Point pos, QColor col)
{
    for (size_t i = 0; i < sides.size(); ++i)
    {
        vector <Point> temp = sides[i];
        for (size_t j = 0; j < temp.size() - 1; ++j)
            putToZBuffer(vector <Point>{temp[j], temp[j + 1]}, col, MathVector(temp[j], pos));
    }
}

void Painter::repaintScene(Camera& cam)
{
    scene->clear();

    QColor col(224, 255, 255);

    for (int i = 0; i < 2000; ++i)
        for (int j = 0; j < 2000; ++j)
        {
            screen[j][i] = INT_MAX;
            buffer[j][i] = col;
        }

    QImage img = QImage(2000, 2000, QImage::Format_RGB32);
    img.fill(QColor(224, 255, 255));

    vector <BaseObject> objects = scene->getObjects();
    Point position = cam.getPos();

    if (objects.size() == 0)
    {
        scene->addPixmap(QPixmap::fromImage(img));
        return;
    }

    Point tempLight = light;

    trans.movePoint(position, cam.getMinusMoved());
    trans.rotatePoint(position, cam.getCenter(), cam.getMinusRotated());
    trans.scalePoint(position, cam.getCenter(), cam.getScaled());

    light = tempLight;

    trans.movePoint(light, cam.getMoved());
    trans.rotatePoint(light, cam.getCenter(), cam.getRotated());
    trans.scalePoint(light, cam.getCenter(), cam.getScaled());

    for (size_t i = 0;i < objects.size(); ++i)
    {
        vector <vector <Point>> tempVis = objects[i].getVisibleSides(cam, trans);
        paintSides(tempVis, position, objects[i].getColor());
    }

    light = tempLight;

    std::thread t1(Painter::fillBuffer, this, 0, 500, ref(img));
    std::thread t2(Painter::fillBuffer, this, 500, 1000, ref(img));
    std::thread t3(Painter::fillBuffer, this, 1000, 1500, ref(img));
    std::thread t4(Painter::fillBuffer, this, 1500, 2000, ref(img));
    t1.join(); t2.join(); t3.join(); t4.join();

    scene->addPixmap(QPixmap::fromImage(img));
}

void Painter::fillBuffer(size_t lower, size_t upper, QImage &img)
{
    for (size_t i = lower; i < upper; ++i)
        for (size_t j = 0; j < 2000; ++j)
            img.setPixelColor(j, i, buffer[j][i].rgb());
}

void Painter::repaintPanel(int ind, QColor col)
{
    QImage img = QImage(120, 110, QImage::Format_RGB32);
    vector <Object> objects = scene->getShapes();

    if (ind == -1)
    {
        for (size_t i = 0; i < objects.size(); ++i)
        {
            img.fill(col);

            for (int i = 0; i < 120; ++i)
                for (int j = 0; j < 120; ++j)
                {
                    panelScr[j][i] = INT_MAX;
                    panelBuf[j][i] = col;
                }

            panel[i]->clear();
            vector <Connecter> cons = objects[i].getConnecters();
            paintPanelElement(BaseObject(objects[i].getCenter(), objects[i].getDimensions(), objects[i].getColor()));
            for (size_t j = 0; j < cons.size(); ++j)
                paintPanelElement(BaseObject(cons[j].getCenter(), cons[j].getDimensions(), cons[j].getColor()));

            for (int i = 0; i < 110; ++i)
                for (int j = 0; j < 120; ++j)
                    img.setPixelColor(j, i, panelBuf[j][i].rgb());

            panel[i]->addPixmap(QPixmap::fromImage(img));
        }
    }
    else
    {
        img.fill(col);

        for (int i = 0; i < 120; ++i)
            for (int j = 0; j < 120; ++j)
            {
                panelScr[j][i] = INT_MAX;
                panelBuf[j][i] = col;
            }

        panel[ind]->clear();
        vector <Connecter> cons = objects[ind].getConnecters();
        paintPanelElement(BaseObject(objects[ind].getCenter(), objects[ind].getDimensions(), objects[ind].getColor()));
        for (size_t j = 0; j < cons.size(); ++j)
            paintPanelElement(BaseObject(cons[j].getCenter(), cons[j].getDimensions(), cons[j].getColor()));

        for (int i = 0; i < 110; ++i)
            for (int j = 0; j < 120; ++j)
                img.setPixelColor(j, i, panelBuf[j][i].rgb());

        panel[ind]->addPixmap(QPixmap::fromImage(img));
    }
}

void Painter::paintPanelElement(BaseObject obj)
{
    Camera cam = Camera();
    cam.setRotated(vector<double>{-0.25, 0.75, 0});
    cam.setScaled(vector <double>{1, 1, 1});
    cam.setMoved(vector <int>{30, -10, 0});
    cam.setPos(Point(50, 50, -1000));
    cam.setCenter(Point(50, 50, 50));
    vector <vector <Point>> sides = obj.getVisibleSides(cam, trans);

    for (size_t i = 0; i < sides.size(); ++i)
    {
        vector <Point> temp = sides[i];

        panelZBuf(temp, FC);

        for (size_t j = 0; j < temp.size() - 1; ++j)
            panelZBuf(vector <Point>{temp[j], temp[j + 1]}, FC);
    }
}

void Painter::panelZBuf(vector<Point> side, QColor col)
{
    MathVector n = MathVector(side[1], side[2]) * MathVector(side[1], side[0]);
    MathVector l(side[0], Point(50, 120, 50));
    MathVector h(side[0], Point(50, 50, -1000));

    int r, g, b;
    col.getRgb(&r, &g, &b);

    n.normalize();
    l.normalize();
    h.normalize();

    double K1, K2;

    K1 = n & l;
    K2 = n & h;

    while (abs(K1) < 100 && abs(K2) < 100)
    {
        K1 *= 10;
        K2 *= 10;
    }

    K1 /= 10;
    K2 /= 10;

    if (r)
        r = (int)(0.8 * r + 0.8 * K1 + 0.65 * K2);
    if (g)
        g = (int)(0.8 * g + 0.8 * K1 + 0.65 * K2);
    if (b)
        b = (int)(0.8 * b + 0.8 * K1 + 0.65 * K2);

    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;

    if (r < 0) r = 0;
    if (g < 0) g = 0;
    if (b < 0) b = 0;

    QColor c(r, g, b);
    int size = side.size();
    int ymax, ymin, ysc, e1, e, i;
    int x[size], y[size];

    for (i = 0; i < size; i++)
        x[i] = int(side[i].x), y[i] = int(side[i].y);

    ymax = ymin = y[0];

    for (int i = 1; i < size; ++i)
    {
        ymax = std::max(ymax, y[i]);
        ymin = std::min(ymin, y[i]);
    }

    ymin = (ymin < 0) ? 0 : ymin;
    ymax = (ymax < 110) ? ymax : 110;

    int ne;
    int x1, x2, xsc1, xsc2;
    double z1, z2, tc, z;

    for (ysc = ymin; ysc < ymax; ysc++)
    {
        ne = 0;
        for (int e = 0; e < size; e++)
        {
            e1 = e + 1;
            if (e1 == size) e1 = 0;
            if (y[e] < y[e1])
            {
                if (y[e1] <= ysc || ysc < y[e]) continue;
            }
            else if (y[e] > y[e1])
            {
                if (y[e1] > ysc || ysc >= y[e]) continue;
            }
            else continue;

            tc = double(y[e] - ysc) / (y[e] - y[e1]);
            if (ne)
            {
                x2 = x[e] + int (tc * (x[e1] - x[e]));
                z2 = side[e].z + tc * (side[e1].z - side[e].z);
            }
            else
            {
                x1 = x[e] + int (tc * (x[e1] - x[e]));
                z1 = side[e].z + tc * (side[e1].z - side[e].z);
                ne = 1;
            }
        }

        if (x2 < x1)
        {
            e = x1;
            x1 = x2;
            x2 = e;
            tc = z1;
            z1 = z2;
            z2 = tc;
        }

        xsc1 = (x1 < 0) ? 0 : x1;
        xsc2 = (x2 < 120) ? x2 : 120;

        for (int xsc = xsc1; xsc < xsc2; xsc++)
        {
            tc = double(x1 - xsc) / (x1 - x2);
            z = z1 + tc * (z2 - z1);

            if (z < panelScr[xsc][ysc])
            {
                panelBuf[xsc][ysc] = c;
                panelScr[xsc][ysc] = z;
            }
        }
    }
}

void Painter::onPanelClicked(int num)
{
    if (lastPanelRepainted == num)
    {
        lastPanelRepainted = -1;
        scene->setPanelIndex(-1);
        repaintPanel(num, QColor("white"));
        return;
    }

    scene->setPanelIndex(num);
    repaintPanel(num, QColor("yellow"));
    if (lastPanelRepainted != -1)
        repaintPanel(lastPanelRepainted, QColor("white"));
    lastPanelRepainted = num;
}
