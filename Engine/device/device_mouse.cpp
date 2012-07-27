
#include "Engine/device/device_mouse.h"

namespace AGS
{
namespace Engine
{
namespace Device
{

CMouseDevice::CMouseDevice()
{
}

CMouseDevice::~CMouseDevice()
{
}

SystemDeviceType CMouseDevice::GetType()
{
    return kSystemDevice_Controller;
}

/*
void CMouseDevice::SetCallback(IMouseGetPosCallback *callback)
{
    _callbackGetMousePos = callback;
}
*/

IMouseCallbacks *CMouseDevice::GetCallbacks()
{
    return _callbacksObj;
}

void CMouseDevice::SetCallbacks(IMouseCallbacks *callbacks_obj)
{
    _callbacksObj = callbacks_obj;
}

} // namespace Device
} // namespace Engine
} // namespace AGS
