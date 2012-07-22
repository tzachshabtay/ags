
#include "Engine/device/allegro_keyboard.h"
#include "util/wgt2allg.h"

namespace AGS
{
namespace Engine
{
namespace Device
{

// Using-declarations
namespace Err = AGS::Common::Core::Err;

CAllegroKeyboard::CAllegroKeyboard()
{
}

CAllegroKeyboard::~CAllegroKeyboard()
{
}

CString CAllegroKeyboard::GetName()
{
    return "alKeyboard";
}

HErr CAllegroKeyboard::Install()
{
    install_keyboard();

    return Err::Nil();
}

HErr CAllegroKeyboard::Remove()
{
    return Err::Nil();
}

HErr CAllegroKeyboard::Poll()
{
    return Err::Nil();
}

} // namespace Device
} // namespace Engine
} // namespace AGS
