
//=============================================================================
//
// Base class for an audio playing device
//
// [IKM] 2012-07-24: draft version
//
//=============================================================================
#ifndef __AGS_EE_DEVICE__DEVICEAUDIO_H
#define __AGS_EE_DEVICE__DEVICEAUDIO_H

#include "Engine/core/system_device.h"

namespace AGS
{
namespace Engine
{
namespace Device
{

// Using-declarations
using AGS::Common::Core::HErr;
using AGS::Engine::Core::SystemDeviceType;
using AGS::Engine::Core::ISystemDevice;

class CAudioDevice : public ISystemDevice
{
public:
    CAudioDevice();
    virtual ~CAudioDevice();

    virtual SystemDeviceType    GetType();
};

} // namespace Device
} // namespace Engine
} // namespace AGS

#endif // __AGS_EE_DEVICE__DEVICEAUDIO_H
