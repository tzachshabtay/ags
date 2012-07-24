
#include "Engine/device/device_audio.h"

namespace AGS
{
namespace Engine
{
namespace Device
{

CAudioDevice::CAudioDevice()
{
}

CAudioDevice::~CAudioDevice()
{
}

SystemDeviceType CAudioDevice::GetType()
{
    return kSystemDevice_Audio;
}

} // namespace Device
} // namespace Engine
} // namespace AGS
