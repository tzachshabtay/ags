
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
    T * arr;
    int count;
    int capacity;
};

} // namespace Util
} // namespace Common
} // namespace AGS

#endif // __AGS_CN_UTIL__PTRARRAY_H
