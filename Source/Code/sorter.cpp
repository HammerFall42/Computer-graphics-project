#include "sorter.h"

Sorter::Sorter() {}

void Sorter::insertionSort(vector <pair <double, int>>& vec)
{
    size_t size = vec.size();

    if (size <= 0) return;

    for (size_t i = 1; i < size; ++i)
        for (size_t j = i; j > 0 && vec[j - 1].first > vec[j].first; --j)
        {
            pair <double, int> p = vec[j - 1];
            vec[j - 1] = vec[j];
            vec[j] = p;
        }
}

void Sorter::xSort(vector <Object>& vec, vector<int> &id, bool invert)
{
    size_t size = vec.size();

    if (size <= 0) return;

    for (size_t i = 1; i < size; ++i)
        for (size_t j = i; j > 0 && vec[j - 1].getCenter().x > vec[j].getCenter().x; --j)
        {
            Object p = vec[j - 1];
            vec[j - 1] = vec[j];
            vec[j] = p;
            int temp = id[j - 1];
            id[j - 1] = id[j];
            id[j] = temp;
        }

    if (invert)
        for (size_t i = 0; i < vec.size() / 2; ++i)
        {
            Object temp = vec[i];
            vec[i] = vec[vec.size() - i - 1];
            vec[vec.size() - i - 1] = temp;
            int tmp = id[i];
            id[i] = id[id.size() - i - 1];
            id[id.size() - i - 1] = tmp;
        }
}

void Sorter::ySort(vector <Object>& vec, vector <int>& id, bool invert)
{
    size_t size = vec.size();

    if (size <= 0) return;

    for (size_t i = 1; i < size; ++i)
        for (size_t j = i; j > 0 && vec[j - 1].getCenter().y > vec[j].getCenter().y; --j)
        {
            Object p = vec[j - 1];
            vec[j - 1] = vec[j];
            vec[j] = p;
            int temp = id[j - 1];
            id[j - 1] = id[j];
            id[j] = temp;
        }

    if (invert)
        for (size_t i = 0; i < vec.size() / 2; ++i)
        {
            Object temp = vec[i];
            vec[i] = vec[vec.size() - i - 1];
            vec[vec.size() - i - 1] = temp;
            int tmp = id[i];
            id[i] = id[id.size() - i - 1];
            id[id.size() - i - 1] = tmp;
        }
}

void Sorter::zSort(vector <Object>& vec, vector<int> &id, bool invert)
{
    size_t size = vec.size();

    if (size <= 0) return;

    for (size_t i = 1; i < size; ++i)
        for (size_t j = i; j > 0 && vec[j - 1].getCenter().z > vec[j].getCenter().z; --j)
        {
            Object p = vec[j - 1];
            vec[j - 1] = vec[j];
            vec[j] = p;
            int temp = id[j - 1];
            id[j - 1] = id[j];
            id[j] = temp;
        }

    if (invert)
        for (size_t i = 0; i < vec.size() / 2; ++i)
        {
            Object temp = vec[i];
            vec[i] = vec[vec.size() - i - 1];
            vec[vec.size() - i - 1] = temp;
            int tmp = id[i];
            id[i] = id[id.size() - i - 1];
            id[id.size() - i - 1] = tmp;
        }
}


void Sorter::Sort(vector <pair <double, int>>& vec)
{
    insertionSort(vec);

    for (size_t i = 0; i < vec.size() / 2; ++i)
    {
        int temp = vec[i].second;
        vec[i].second = vec[vec.size() - i - 1].second;
        vec[vec.size() - i - 1].second = temp;
    }
}
