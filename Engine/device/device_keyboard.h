
//=============================================================================
//
// Base class for a keyboard device
//
// [IKM] 2012-07-19: draft version
//
//=============================================================================
#ifndef __AGS_EE_DEVICE__DEVICEKEYBOARD_H
#define __AGS_EE_DEVICE__DEVICEKEYBOARD_H

#include "Engine/device/system_device.h"

namespace AGS
{
namespace Engine
{
namespace Device
{

class CKeyboardDevice : public ISystemDevice
{
public:
    CKeyboardDevice();
    virtual ~CKeyboardDevice();

    virtual SystemDeviceType    GetType();
};

} // namespace Device
} // namespace Engine
} // namespace AGS

#endif // __AGS_EE_DEVICE__DEVICEKEYBOARD_H
