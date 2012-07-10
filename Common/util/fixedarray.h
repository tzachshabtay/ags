
//=============================================================================
//
// [IKM] 2012-07-09: placeholder for the fixed array class
//
//=============================================================================
#ifndef __AGS_CN_UTILS__FIXEDARRAY_H
#define __AGS_CN_UTILS__FIXEDARRAY_H

namespace AGS
{
namespace Common
{
namespace Util
{

template<typename T> struct CFixedArray
{
    T * arr;
    int length;
};

} // namespace Util
} // namespace Common
} // namespace AGS

#endif // __AGS_CN_UTILS__FIXEDARRAY_H
