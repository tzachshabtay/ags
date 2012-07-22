
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
};

} // namespace Device
} // namespace Engine
} // namespace AGS

#endif // __AGS_EE_DEVICE__ALLEGROMOUSE_H
