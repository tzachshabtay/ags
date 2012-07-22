
//=============================================================================
//
// Game assets manager
//
// [IKM] 2012-07-21: draft version
//
//=============================================================================
#ifndef __AGS_EE_GAME__ASSETSMANAGER_H
#define __AGS_EE_GAME__ASSETSMANAGER_H

#include "Common/core/err.h"
#include "Common/util/string.h"
#include "Engine/core/engine_setup.h"

namespace AGS
{

// Forward-declaration
namespace Common
{
namespace Util
{
class CString;
} // namespace Util
} // namespace Common

namespace Engine
{

// Forward-declaration
namespace Core
{
struct CEngineSetup;
} // namespace Core

namespace Game
{

// Using-declarations
using AGS::Common::Core::HErr;
using AGS::Common::Util::CString;
using AGS::Engine::Core::CEngineSetup;

class CAssetsManager
{
public:
    CAssetsManager(CEngineSetup &setup, const CString &data_file_name, const CString &data_directory);
    ~CAssetsManager();

    HErr    RegisterGameData();
    CString GetDataFileName() const;
    CString GetDataDirectory() const;

protected:

    bool    SetGameDataFileName();
    int     InitGameDataExternal();
    HErr    InitGameDataInternal();

private:

    CEngineSetup  &_theSetup;
    CString       _gameDataFileName;
    CString       _gameDataDirectory;
};

} // namespace Game
} // namespace Engine
} // namespace AGS

#endif // __AGS_EE_GAME__ASSETSMANAGER_H
