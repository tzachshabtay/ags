
//=============================================================================
//
// [IKM] 2012-07-09: placeholder for the ptr array class
//
//=============================================================================
#ifndef __AGS_CN_UTIL__PTRARRAY_H
#define __AGS_CN_UTIL__PTRARRAY_H

namespace AGS
{
namespace Common
{
namespace Util
{

template<typename T> struct CPtrArray
{
    T **arr;
    int count;
    int capacity;

    void Add(const T *el) {}
    int  GetCount() const { return count; }
    void SetCount(int count) {}
};

} // namespace Util
} // namespace Common
} // namespace AGS

#endif // __AGS_CN_UTIL__PTRARRAY_H
