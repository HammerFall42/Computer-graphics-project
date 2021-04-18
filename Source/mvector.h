#ifndef MVECTOR_H
#define MVECTOR_H

#include <stdlib.h>
#include <stdarg.h>
#include <string>
#include <memory>

#include "error.h"

using namespace std;

template <typename ValueType>
class MVector
{
public:
    MVector();
    MVector(size_t size);
    MVector(const MVector<ValueType>&mvector);
    ~MVector();

    ValueType& operator[](size_t index);
    MVector<ValueType>& operator=(const MVector<ValueType>& mvector);

    void push_back(ValueType element);
    void del(size_t index);
    void clear() {vsize = 0;}
    void reverse();
    size_t size() {return vsize;}

private:
    ValueType *p;
    size_t vsize;
    size_t allocated;
};

#endif // MVECTOR_H
