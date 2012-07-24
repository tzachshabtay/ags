
#include "Engine/device/device_keyboard.h"

namespace AGS
{
namespace Engine
{
namespace Device
{

CKeyboardDevice::CKeyboardDevice()
{
}

CKeyboardDevice::~CKeyboardDevice()
{
}

SystemDeviceType CKeyboardDevice::GetType()
{
    return kSystemDevice_Controller;
}

} // namespace Device
} // namespace Engine
} // namespace AGS
