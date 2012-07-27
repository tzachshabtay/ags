
//=============================================================================
//
// Allegro mouse: a wrapper around allegro mouse functions
//
// [IKM] 2012-07-19: draft version
//
//=============================================================================
#ifndef __AGS_EE_DEVICE__ALLEGROMOUSE_H
#define __AGS_EE_DEVICE__ALLEGROMOUSE_H

#include "Engine/device/device_mouse.h"

namespace AGS
{
namespace Engine
{
namespace Device
{

// Using-declarations
using AGS::Common::Core::HErr;
using AGS::Common::Util::CString;

class CAllegroMouse : public CMouseDevice
{
public:
    CAllegroMouse();
    virtual ~CAllegroMouse();

    virtual CString             GetName();

    virtual HErr                Install();
    virtual HErr                Remove();
    virtual HErr                Poll();

    virtual MouseButtons        GetInstalledButtons() const;
    virtual MouseButtons        GetPressedButtons() const;
    virtual HErr                SetConfineBox(const CRect &rc);
    virtual CRect               GetConfineBox() const;
    virtual HErr                SetMouseLimits(const CRect &rc);
    virtual CRect               GetMouseLimits() const;
    virtual HErr                SetMousePos(const CPoint &pt);
    virtual CPoint              GetMousePos() const;

protected:

    void                        UpdateMousePosition();
    void                        UpdateMouseButtons();

private:

    CRect           _confineBox;
    CRect           _boundsBox;
    CPoint          _mousePos;
    CPoint          _mouseOldPos;
    MouseButtons    _buttonsInstalled;
    MouseButtons    _buttonsPressed;
    MouseButtons    _buttonsWerePressed;

    bool            _disableGetMousePos;
    bool            _ignoreBounds;
};

} // namespace Device
} // namespace Engine
} // namespace AGS

#endif // __AGS_EE_DEVICE__ALLEGROMOUSE_H
