
//=============================================================================
//
// Base class for a screen device
//
// [IKM] 2012-07-19: draft version
//
//=============================================================================
#ifndef __AGS_EE_DEVICE__DEVICESCREEN_H
#define __AGS_EE_DEVICE__DEVICESCREEN_H

#include "Engine/device/system_device.h"

// Forward declarations
class IGraphicsDriver;
class GFXFilter;

namespace AGS
{
namespace Engine
{
namespace Device
{

class CScreenDevice : public ISystemDevice
{
public:
    CScreenDevice();
    virtual ~CScreenDevice();

    virtual SystemDeviceType    GetType();

    IGraphicsDriver *GetGraphicsDriver();
    GFXFilter       *GetGraphicsFilter();

    virtual HErr    InitGraphicsMode(int32_t width, int32_t height, int8_t bpp, int32_t hz, bool windowed) = 0;
    // TODO: rename?
    virtual HErr    SetMultitasking (bool on) = 0;

private:

    IGraphicsDriver *_gfxDriver;
    GFXFilter       *_gfxFilter;
};

} // namespace Device
} // namespace Engine
} // namespace AGS

#endif // __AGS_EE_DEVICE__DEVICESCREEN_H
