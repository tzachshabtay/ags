
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

// These are from clib32
#define PR_DATAFIRST 1
#define PR_FILEFIRST 2

namespace AGS
{
// Forward-declaration
namespace Common
{
namespace Util
{
class CStream;
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
using AGS::Common::Util::CStream;
using AGS::Common::Util::CString;
using AGS::Engine::Core::CEngineSetup;

enum AssetsSearchPriority
{
    // TODO: rename this to something more obvious
    kAssetsPriority_Data = PR_DATAFIRST,
    kAssetsPriority_File = PR_FILEFIRST
};

class CAssetsManager
{
public:
    CAssetsManager(const CString &data_file_name, const CString &data_directory);
    ~CAssetsManager();

    HErr    RegisterGameData();
    CString GetDataDirectory() const;
    CString GetDataFileName() const;
    CString GetMusicFileName() const;
    CString GetSpeechFileName() const;

    AssetsSearchPriority GetSearchPriority() const;
    bool                 SetSearchPriority(AssetsSearchPriority pr);

    CStream *OpenAsset(const CString &data_file, const CString &asset_file, const CString &mode);


    bool    _seperateMusicLib; // CHECKME: probably redundant

protected:

    bool    SetGameDataFileName();
    int     InitGameDataExternal();
    HErr    InitGameDataInternal();
    HErr    RegisterAdditionalDataFiles();
    HErr    RegisterDataFile(const CString &data_file, CString &actual_file_path);

private:

    CString _gameDataDirectory;
    CString _gameDataFileName;
    CString _musicFileName;
    CString _speechFileName;

    CString _currentDataFile;
};

} // namespace Game
} // namespace Engine
} // namespace AGS

#endif // __AGS_EE_GAME__ASSETSMANAGER_H
