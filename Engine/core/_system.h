
//=============================================================================
//
// System class
//
// [IKM] 2012-07-19: draft version
//
//=============================================================================
#ifndef __AGS_EE_CORE__SYSTEM_H
#define __AGS_EE_CORE__SYSTEM_H

#include "Common/util/ptrarray.h"
#include "Engine/device/device_keyboard.h"
#include "Engine/device/device_mouse.h"
#include "Engine/device/device_screen.h"
#include "ac/dynobj/scriptsystem.h"

namespace AGS
{
namespace Engine
{

// Forward declarations
namespace Device
{
class CKeyboardDevice;
class CMouseDevice;
class CScreenDevice;
} // namespace Device

namespace Core
{

// Forward declarations
class ISystemDevice;

// Using-declarations
using AGS::Common::Util::CPtrArray;
using AGS::Engine::Device::CKeyboardDevice;
using AGS::Engine::Device::CMouseDevice;
using AGS::Engine::Device::CScreenDevice;

class CSystem
{
public:
    CSystem();
    ~CSystem();

    HErr                    Initialize();
    void                    Shutdown();

    /*const*/ ScriptSystem  &GetScriptSystem() /*const*/;

    CKeyboardDevice         *GetKeyboard();
    CMouseDevice            *GetMouse();
    CScreenDevice           *GetScreen();

protected:

    HErr                    InstallDevice(ISystemDevice *device);
    HErr                    RemoveDevice(ISystemDevice *device);

private:

    ScriptSystem        _scriptSystem;
    CKeyboardDevice     *_deviceKeyboard;
    CMouseDevice        *_deviceMouse;
    CScreenDevice       *_deviceScreen;
};

} // namespace Core
} // namespace Engine
} // namespace AGS

#endif // __AGS_EE_CORE__SYSTEM_H
