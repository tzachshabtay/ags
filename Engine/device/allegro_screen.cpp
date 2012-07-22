
#include "Engine/device/allegro_screen.h"

namespace AGS
{
namespace Engine
{
namespace Device
{

// Using-declarations
namespace Err = AGS::Common::Core::Err;

CAllegroScreen::CAllegroScreen()
{
}

CAllegroScreen::~CAllegroScreen()
{
}

CString CAllegroScreen::GetName()
{
    return "alScreen";
}

HErr CAllegroScreen::Install()
{
    return Err::Nil();
}

HErr CAllegroScreen::Remove()
{
    return Err::Nil();
}

HErr CAllegroScreen::Poll()
{
    return Err::Nil();
}

HErr CAllegroScreen::InitGraphicsMode(int32_t width, int32_t height, int8_t bpp, int32_t hz, bool windowed)
{
    return Err::Nil();
}

HErr CAllegroScreen::SetMultitasking (bool on)
{
    return Err::Nil();
}

} // namespace Device
} // namespace Engine
} // namespace AGS
