
//=============================================================================
//
// Base class for a mouse device
//
// [IKM] 2012-07-19: draft version
//
//=============================================================================
#ifndef __AGS_EE_DEVICE__DEVICEMOUSE_H
#define __AGS_EE_DEVICE__DEVICEMOUSE_H

#include "Engine/device/system_device.h"

namespace AGS
{
namespace Engine
{
namespace Device
{

class CMouseDevice : public ISystemDevice
{
public:
    CMouseDevice();
    virtual ~CMouseDevice();

    virtual SystemDeviceType    GetType();
};

} // namespace Device
} // namespace Engine
} // namespace AGS

#endif // __AGS_EE_DEVICE__DEVICEMOUSE_H
