
#include "Engine/device/device_screen.h"

namespace AGS
{
namespace Engine
{
namespace Device
{

CScreenDevice::CScreenDevice()
{
}

CScreenDevice::~CScreenDevice()
{
}

SystemDeviceType CScreenDevice::GetType()
{
    return kSystemDevice_Screen;
}

IGraphicsDriver *CScreenDevice::GetGraphicsDriver()
{
    return _gfxDriver;
}

} // namespace Device
} // namespace Engine
} // namespace AGS
