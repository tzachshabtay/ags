
//=============================================================================
//
// [IKM] 2012-07-27: placeholder for the set class
//
//=============================================================================
#ifndef __AGS_CN_UTIL__SET_H
#define __AGS_CN_UTIL__SET_H

#include "Common/core/types.h"

namespace AGS
{
namespace Common
{
namespace Util
{

template<typename T> struct CSet
{
    T * arr;
 
    void Add(T &t) {}
    void Remove(T &t) {}
    int32_t GetCount() const;
    T &operator[](int index) { return arr[index]; }
};

} // namespace Util
} // namespace Common
} // namespace AGS

#endif // __AGS_CN_UTIL__SET_H
