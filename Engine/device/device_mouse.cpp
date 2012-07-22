
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
    return SystemDeviceType::kSystemDevice_Controller;
}

} // namespace Device
} // namespace Engine
} // namespace AGS
