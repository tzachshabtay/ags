
#include "Engine/core/_system.h"
#include "Engine/device/allegro_audio.h"
#include "Engine/device/allegro_keyboard.h"
#include "Engine/device/allegro_mouse.h"
#include "Engine/device/allegro_screen.h"

namespace AGS
{
namespace Engine
{
namespace Core
{

// Using-declarations
using AGS::Engine::Device::CAllegroAudio;
using AGS::Engine::Device::CAllegroKeyboard;
using AGS::Engine::Device::CAllegroMouse;
using AGS::Engine::Device::CAllegroScreen;
namespace Err = AGS::Common::Core::Err;

CSystem::CSystem()
{
}

CSystem::~CSystem()
{
}

/*const*/ ScriptSystem &CSystem::GetScriptSystem() /*const*/
{
    return _scriptSystem;
}

CKeyboardDevice *CSystem::GetKeyboard()
{
    return _deviceKeyboard;
}

CMouseDevice *CSystem::GetMouse()
{
    return _deviceMouse;
}

CScreenDevice *CSystem::GetScreen()
{
    return _deviceScreen;
}

HErr CSystem::Initialize()
{
    HErr err;
    ISystemDevice *device;

    device = new CAllegroKeyboard();
    err = InstallDevice(device);
    if (!err->IsNil()) {
        delete device;
        return err;
    }
    _deviceKeyboard = (CKeyboardDevice*)device;

    device = new CAllegroMouse();
    err = InstallDevice(device);
    if (!err->IsNil()) {
        delete device;
        return err;
    }
    _deviceMouse = (CMouseDevice*)device;

    device = new CAllegroScreen();
    err = InstallDevice(device);
    if (!err->IsNil()) {
        delete device;
        return err;
    }
    _deviceScreen = (CScreenDevice*)device;

	device = new CAllegroAudio();
    err = InstallDevice(device);
    if (!err->IsNil()) {
        delete device;
        return err;
    }
    _deviceAudio = (CAudioDevice*)device;

	return Err::Nil();
}

void CSystem::Shutdown()
{
	RemoveDevice(_deviceAudio);
    RemoveDevice(_deviceScreen);
    RemoveDevice(_deviceKeyboard);
    RemoveDevice(_deviceMouse);
}

HErr CSystem::InstallDevice(ISystemDevice *device)
{
    return device->Install();
}

HErr CSystem::RemoveDevice(ISystemDevice *device)
{
    HErr err = Err::Nil();
    if (device)
    {
        err = device->Remove();
        delete device;
    }
    return err;
}

} // namespace Core
} // namespace Engine
} // namespace AGS
