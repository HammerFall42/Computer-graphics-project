#ifndef OBJECT_H
#define OBJECT_H

#include "baseobject.h"
#include "connecter.h"

class Object : public BaseObject
{
public:
    Object(Point c, vector<int> d, QColor col);
    ~Object() {}

    void move(vector <int> d);

    virtual void setColor(QColor col);
    void addConnecter(Connecter con) {connecters.push_back(con);}
    void setCollideCenter(Point c) {collideCenter = c;}

    Point getCollideCenter() {return collideCenter;}
    vector <Connecter> getConnecters() {return connecters;}
    static int getModifier() {return modifier;}

    bool isInside(Camera& view, Transformator& tr, pair<int, int> p);

private:
    vector <Connecter> connecters;
    Point collideCenter;
};

#endif // OBJECT_H
