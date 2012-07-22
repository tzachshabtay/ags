
#include "util/wgt2allg.h"
#include "Common/ac/common_defines.h"
#include "Engine/device/allegro_mouse.h"
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
#ifdef _DEBUG
    // Quantify fails with the mouse for some reason
    minstalled();
#else
    if (minstalled()==0) {
        AGSPlatformDriver *platform = AGSPlatformDriver::GetDriver();
        platform->DisplayAlert(platform->GetNoMouseErrorString());
        return Err::FromCode(EXIT_NORMAL);
    }
#endif // DEBUG

    return Err::Nil();
}

HErr CAllegroMouse::Remove()
{
    return Err::Nil();
}

HErr CAllegroMouse::Poll()
{
    return Err::Nil();
}

} // namespace Device
} // namespace Engine
} // namespace AGS
