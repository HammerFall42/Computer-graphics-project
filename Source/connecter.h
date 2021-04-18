#ifndef CONNECTER_H
#define CONNECTER_H

#include "baseobject.h"

class Connecter : public BaseObject
{
public:
    Connecter(Point c, vector <int> d, QColor col);
    ~Connecter() {}

    bool isUsed() {return used;}
    void move(vector <int> d);

private:
    bool used;
};

#endif // CONNECTER_H
