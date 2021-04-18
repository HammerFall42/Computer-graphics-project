#ifndef MVECTOR_HPP
#define MVECTOR_HPP

#include "mvector.h"

template <typename ValueType>
MVector<ValueType>::MVector()
{
    vsize = 0;
    allocated = 0;

    p = new ValueType[5];

    if (!p)
    {
        throw MemoryError();
        return;
    }

    allocated = 5;
}

template <typename ValueType>
MVector<ValueType>::MVector(size_t size)
{
    vsize = 0;

    p = new ValueType[size];

    if (!p)
    {
        throw MemoryError();
        return;
    }

    allocated = size;
}

template <typename ValueType>
MVector<ValueType>::MVector(const MVector<ValueType> & mvector)
{
    vsize = 0;
    size_t size = mvector.vsize;
    allocated = size;
    p = new ValueType[allocated];

    for (size_t i = 0; i < size; ++i)
    {
        p[i] = *(mvector.p + i);
        vsize++;
    }
}

template <typename ValueType>
MVector<ValueType>::~MVector() {}

template <typename ValueType>
void MVector<ValueType>::push_back(ValueType element)
{
    if (vsize == allocated)
    {
        allocated *= 2;
        ValueType *newP = new ValueType[allocated];

        if (!newP)
        {
            throw MemoryError();
            return;
        }

        for (size_t i = 0; i < vsize; ++i)
            newP[i] = p[i];

        p = newP;
    }

    p[vsize] = element;
    vsize++;
}

template <typename ValueType>
void MVector<ValueType>::del(size_t index)
{
    if (index > vsize)
    {
        throw RangeError();
        return;
    }

    for (size_t i = index; i < vsize - 1; ++i)
        p[i] = p[i + 1];

    vsize--;
}

template <typename ValueType>
ValueType& MVector<ValueType>::operator[](size_t index)
{
    if (index > vsize)
        throw RangeError();

    return p[index];
}

template <typename ValueType>
MVector<ValueType>& MVector<ValueType>::operator=(const MVector& mvector)
{
    vsize = 0;
    size_t size = mvector.vsize;
    allocated = size;
    p = new ValueType[allocated];

    for (size_t i = 0; i < size; ++i)
    {
        p[i] = *(mvector.p + i);
        vsize++;
    }

    return *this;
}


template <typename ValueType>
void MVector<ValueType>::reverse()
{
    for (size_t i = 0; i < vsize / 2; ++i)
    {
        ValueType temp = p[i];
        p[i] = p[vsize - i - 1];
        p[vsize - i - 1] = temp;
    }
}


#endif // MVECTOR_HPP
