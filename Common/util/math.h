
//=============================================================================
//
// Various functions and structs to help with math and geometry calculations
//
//=============================================================================
#ifndef __AGS_CN_UTIL__MATH_H
#define __AGS_CN_UTIL__MATH_H

#include "Common/core/types.h"

namespace AGS
{
namespace Common
{
namespace Util
{

struct CPoint
{
    int32_t X;
    int32_t Y;

    inline bool operator ==(const CPoint &pt) const
    {
        return X == pt.X && Y == pt.Y;
    }

    inline bool operator !=(const CPoint &pt) const
    {
        return !(*this == pt);
    }
};

struct CRect
{
    int32_t Left;
    int32_t Top;
    int32_t Right;
    int32_t Bottom;

    CRect()
    {
        Left = Top = Right = Bottom = 0;
    }

    CRect(int32_t left, int32_t top, int32_t right, int32_t bottom)
    {
        Left    = left;
        Top     = top;
        Right   = right;
        Bottom  = bottom;
    }
};

} // namespace Util
} // namespace Common
} // namespace AGS

#endif // __AGS_CN_UTIL__MATH_H
