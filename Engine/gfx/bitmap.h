
//=============================================================================
//
// Bitmap class
//
// [IKM] 2012-07-27: placeholder
//
//=============================================================================
#ifndef __AGS_EE_GFX__BITMAP_H
#define __AGS_EE_GFX__BITMAP_H

namespace AGS
{
namespace Engine
{
namespace Graphics
{

class CBitmap
{
    BITMAP                  *bmp;
    IDriverDependantBitmap  *ddb;
};

} // namespace Graphics
} // namespace Engine
} // namespace AGS

#endif // __AGS_EE_GFX__BITMAP_H
