
#include "Common/ac/common_defines.h"
#include "Engine/device/allegro_mouse.h"
#include "util/wgt2allg.h"
#include "Engine/device/mousew32.h"
#include "Engine/platform/base/agsplatformdriver.h"

namespace AGS
{
namespace Engine
{
namespace Device
{

// Using-declarations
namespace Err = AGS::Common::Core::Err;

CAllegroMouse::CAllegroMouse()
{
}

CAllegroMouse::~CAllegroMouse()
{
}

CString CAllegroMouse::GetName()
{
    return "alMouse";
}

HErr CAllegroMouse::Install()
{
    int buttons_number = install_mouse();
#ifdef _DEBUG
    // Quantify fails with the mouse for some reason
    if (false)
#else
    if (buttons_number < 1)
#endif
    {
        AGSPlatformDriver::GetDriver()->DisplayAlert(platform->GetNoMouseErrorString());
        return Err::FromCode(EXIT_NORMAL);
    }

    SetConfineBox(CRect(0, 0, 319, 199));  // use 320x200 co-ord system
    SetMouseLimits(CRect(0, 0, 99999, 99999)); // now, all we have to do is to find a monitor large enough...

    return Err::Nil();
}

HErr CAllegroMouse::Remove()
{
    remove_mouse();
    return Err::Nil();
}

HErr CAllegroMouse::Poll()
{
    poll_mouse();

    UpdateMousePosition();
    UpdateMouseButtons();

    return Err::Nil();
}

MouseButtons CAllegroMouse::GetInstalledButtons() const
{
    return _buttonsInstalled;
}

MouseButtons CAllegroMouse::GetPressedButtons() const
{
    return _buttonsPressed;
}

HErr CAllegroMouse::SetConfineBox(const CRect &rc)
{
    set_mouse_range(rc.Left, rc.Top, rc.Right, rc.Bottom);
    _confineBox = rc;
    return Err::Nil();
}

CRect CAllegroMouse::GetConfineBox() const
{
    return _confineBox;
}

HErr CAllegroMouse::SetMouseLimits(const CRect &rc)
{
    // like graphconfine, but don't actually pass it to the driver
    // - stops the Windows cursor showing when out of the area
    _boundsBox = rc;
    return Err::Nil();
}

CRect CAllegroMouse::GetMouseLimits() const
{
    return _boundsBox;
}

HErr CAllegroMouse::SetMousePos(const CPoint &pt)
{
    position_mouse(pt.X, pt.Y);
    _mousePos = pt;
    return Err::Nil();
}

CPoint CAllegroMouse::GetMousePos() const
{
    return _mousePos;
}

void CAllegroMouse::UpdateMousePosition()
{
    if (!_disableGetMousePos)
    {
        _mousePos.X = mouse_x;
        _mousePos.Y = mouse_y;
    }

    if (!_ignoreBounds)
    {
        if (_mousePos.X < _boundsBox.Left)
        {
            _mousePos.X = _boundsBox.Left;
            SetMousePos(_mousePos);
        }
        if (_mousePos.Y < _boundsBox.Top)
        {
            _mousePos.Y = _boundsBox.Top;
            SetMousePos(_mousePos);
        }
        if (_mousePos.X > _boundsBox.Right)
        {
            _mousePos.X = _boundsBox.Right;
            SetMousePos(_mousePos);
        }
        if (_mousePos.Y > _boundsBox.Bottom)
        {
            _mousePos.Y = _boundsBox.Bottom;
            SetMousePos(_mousePos);
        }
    }

    // Event: mouse moved
    if (_mousePos != _mouseOldPos)
    {
        if (_callbacksObj)
        {
            _callbacksObj->OnMouseMove(this, _mouseOldPos, _mousePos);
        }
    }

    /* TODO: move to CSystem callback (calls GFXFilter),
    // also should probably affect CMouseCursor drawing, not mouse device
    if ((callback) && (!_disableGetMousePos))
    {
        callback->AdjustPosition(&_mousePos.X, &_mousePos.Y);
    }
    */

    _mouseOldPos = _mousePos;
}

void CAllegroMouse::UpdateMouseButtons()
{
    _buttonsPressed = kMB_None;

    int btn_down = mouse_b;
    if (btn_down & 1)   _buttonsPressed |= kMB_Left;
    if (btn_down & 2)   _buttonsPressed |= kMB_Right;
    if (btn_down & 4)   _buttonsPressed |= kMB_Middle;

    if (_callbacksObj)
    {
        MouseButtons btn_down   = _buttonsPressed & (~_buttonsWerePressed);
        MouseButtons btn_up     = _buttonsWerePressed & (~_buttonsPressed);

        if (btn_down)
        {
            _callbacksObj->OnMouseBtnDown(this, btn_down);
        }
        if (btn_up)
        {
            _callbacksObj->OnMouseBtnUp(this, btn_up);
        }
        // TODO: click and double-click events require timing
    }

    _buttonsWerePressed = _buttonsPressed;
}

} // namespace Device
} // namespace Engine
} // namespace AGS
