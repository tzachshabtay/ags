
//=============================================================================
//
// Mouse callbacks interface
//
//=============================================================================
#ifndef __AGS_EE_DEVICE__MOUSECALLBACKS_H
#define __AGS_EE_DEVICE__MOUSECALLBACKS_H

#include "Common/util/math.h"
#include "Engine/device/mouse_defines.h"

namespace AGS
{
namespace Engine
{
namespace Device
{

// Forward declarations
class CMouseDevice;

// Using-declarations
using AGS::Common::Util::CPoint;
using AGS::Common::Util::CRect;

class IMouseCallbacks
{
public:
    virtual ~IMouseCallbacks(){}

    virtual void OnMouseMove        (CMouseDevice *mouse, const CPoint &old_pos, const CPoint &new_pos) = 0;
    virtual void OnMouseBtnDown     (CMouseDevice *mouse, MouseButtons btn) = 0;
    virtual void OnMouseBtnUp       (CMouseDevice *mouse, MouseButtons btn) = 0;
    virtual void OnMouseBtnClick    (CMouseDevice *mouse, MouseButtons btn) = 0;
    virtual void OnMouseBtnDblClick (CMouseDevice *mouse, MouseButtons btn) = 0;
};

} // namespace Device
} // namespace Engine
} // namespace AGS

#endif // __AGS_EE_DEVICE__MOUSECALLBACKS_H
