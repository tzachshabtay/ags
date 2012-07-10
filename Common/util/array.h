
//=============================================================================
//
// [IKM] 2012-07-09: placeholder for the array class
//
//=============================================================================
#ifndef __AGS_CN_UTILS__ARRAY_H
#define __AGS_CN_UTILS__ARRAY_H

namespace AGS
{
namespace Common
{
namespace Util
{

template<typename T> struct CArray
{
    T * arr;
    int count;
    int capacity;
};

} // namespace Util
} // namespace Common
} // namespace AGS

#endif // __AGS_CN_UTILS__ARRAY_H
