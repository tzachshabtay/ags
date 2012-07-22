
//=============================================================================
//
// Allegro keyboard: a wrapper around allegro keyboard functions
//
// [IKM] 2012-07-19: draft version
//
//=============================================================================
#ifndef __AGS_EE_DEVICE__ALLEGROKEYBOARD_H
#define __AGS_EE_DEVICE__ALLEGROKEYBOARD_H

#include "Engine/device/device_keyboard.h"

namespace AGS
{
namespace Engine
{
namespace Device
{

// Using-declarations
using AGS::Common::Core::HErr;
using AGS::Common::Util::CString;

class CAllegroKeyboard : public CKeyboardDevice
{
public:
    CAllegroKeyboard();
    virtual ~CAllegroKeyboard();

    virtual CString             GetName();

    virtual HErr                Install();
    virtual HErr                Remove();
    virtual HErr                Poll();
};

} // namespace Device
} // namespace Engine
} // namespace AGS

#endif // __AGS_EE_DEVICE__ALLEGROKEYBOARD_H
