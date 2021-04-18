#ifndef SORTER_H
#define SORTER_H

#include <vector>
#include <algorithm>
#include "object.h"

using std::pair;
using std::vector;

class Sorter
{
public:
    Sorter();
    ~Sorter() {}

    void Sort(vector <pair <double, int>>& vec);
    static void xSort(vector <Object>& vec, vector <int>& id, bool invert);
    static void ySort(vector <Object>& vec, vector <int>& id, bool invert);
    static void zSort(vector <Object>& vec, vector <int>& id, bool invert);

private:
    void insertionSort(vector <pair <double, int>>& vec);
};

#endif // SORTER_H
