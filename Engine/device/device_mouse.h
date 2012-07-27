
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
#include "Engine/device/mouse_callbacks.h"

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

    virtual MouseButtons        GetInstalledButtons() const = 0;
    virtual MouseButtons        GetPressedButtons() const = 0;
    virtual HErr                SetConfineBox(const CRect &rc) = 0;
    virtual CRect               GetConfineBox() const = 0;
    virtual HErr                SetMouseLimits(const CRect &rc) = 0;
    virtual CRect               GetMouseLimits() const = 0;
    virtual HErr                SetMousePos(const CPoint &pt) = 0;
    virtual CPoint              GetMousePos() const = 0;

    //void                        SetCallback(IMouseGetPosCallback *callback);
    void                        SetCallbacks(IMouseCallbacks *callbacks_obj);
    IMouseCallbacks             *GetCallbacks();

protected:
    //IMouseGetPosCallback        *_callbackGetMousePos;
    IMouseCallbacks             *_callbacksObj;
};

} // namespace Device
} // namespace Engine
} // namespace AGS

#endif // __AGS_EE_DEVICE__DEVICEMOUSE_H
