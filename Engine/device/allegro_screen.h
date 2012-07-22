
//=============================================================================
//
// Allegro screen: a wrapper around allegro screen and gfx mode functions
//
// [IKM] 2012-07-19: draft version
//
//=============================================================================
#ifndef __AGS_EE_DEVICE__ALLEGROSCREEN_H
#define __AGS_EE_DEVICE__ALLEGROSCREEN_H

#include "Engine/device/device_screen.h"

namespace AGS
{
namespace Engine
{
namespace Device
{

// Using-declarations
using AGS::Common::Util::CString;

class CAllegroScreen : public CScreenDevice
{
public:
    CAllegroScreen();
    virtual ~CAllegroScreen();

    virtual CString             GetName();

    virtual HErr                Install();
    virtual HErr                Remove();
    virtual HErr                Poll();

    virtual HErr InitGraphicsMode(int32_t width, int32_t height, int8_t bpp, int32_t hz, bool windowed);
    virtual HErr SetMultitasking (bool on);
};

} // namespace Device
} // namespace Engine
} // namespace AGS

#endif // __AGS_EE_DEVICE__ALLEGROSCREEN_H
