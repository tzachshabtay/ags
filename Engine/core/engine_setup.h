
//=============================================================================
//
// AGS Engine Setup data structure
//
// [IKM] 2012-07-09: draft version
//
//=============================================================================
#ifndef __AGS_EE_CORE__ENGINESETUP_H
#define __AGS_EE_CORE__ENGINESETUP_H

#include "Common/util/keyvaluetree.h"


namespace AGS
{

// Forward declarations
namespace Common
{
namespace Util
{
class CStream;
class CKeyValueTree;
} // namespace Util
} // namespace Common

namespace Engine
{

// Forward declarations
namespace Util
{
class CCmdArgs;
} // namespace Util

namespace Core
{

// Using-declarations
using AGS::Common::Core::HErr;
using AGS::Common::Util::CString;
using AGS::Common::Util::CStream;
using AGS::Engine::Util::CCmdArgs;

// [IKM] 2012-07-09: I put all the contents of original GameSetup structure here;
// althought *probably* some of these values should be later moved to some kind
// of CGameSetup structure.
struct CEngineSetup
{
public:
    int32_t Digicard;
    int32_t Midicard;
    int32_t ModPlayer;
    int32_t TextHeight;
    int32_t MP3Player;
    int32_t WantLetterbox;
    int32_t Windowed;
    int32_t BaseWidth;
    int32_t BaseHeight;
    int16_t Refresh;
    int8_t  NoSpeechPack;
    int8_t  EnableAntialiasing;
    int8_t  ForceHicolorMode;
    int8_t  DisableExceptionHandling;
    int8_t  EnableSideBorders;

    CString DataFilesDir;
    CString MainDataFilename;
    CString Translation;
    CString GfxFilterID;
    CString GfxDriverID;

    int32_t ForceLetterbox;
    CString ReplayFile;
    int     DebugFlags;
    bool    MustChangeToGameDataDirectory;

    CEngineSetup();
    void SetDefaults();
    void OverrideByCmdArgs(const CCmdArgs &cmd_args);
    void OverrideByPlatform();
    // Read and write binary data
    HErr Read (CStream *in);
    HErr Write(CStream *out);
    // Read and write from/to key-value structure
    HErr ReadFromTree (Common::Util::CKeyValueTree *tree);
    HErr WriteToTree(Common::Util::CKeyValueTree *tree);
};

} // namespace Core
} // namespace Engine
} // namespace AGS

#endif // __AGS_EE_CORE__ENGINESETUP_H
