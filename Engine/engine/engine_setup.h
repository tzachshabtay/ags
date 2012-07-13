
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
struct CStream;
class CKeyValueTree;
} // namespace Util
} // namespace Common

namespace Engine
{

// Using-declarations
using AGS::Common::Core::HErr;
using AGS::Common::Util::CString;
using AGS::Common::Util::CStream;

// [IKM] 2012-07-09: I put all the contents of original GameSetup structure here;
// althought *probably* some of these values should be later moved to some kind
// of CGameSetup structure.
struct CEngineSetup
{
public:
    int32 Digicard;
    int32 Midicard;
    int32 ModPlayer;
    int32 TextHeight;
    int32 MP3Player;
    int32 WantLetterbox;
    int32 Windowed;
    int32 BaseWidth;
    int32 BaseHeight;
    int16 Refresh;
    int8  NoSpeechPack;
    int8  EnableAntialiasing;
    int8  ForceHicolorMode;
    int8  DisableExceptionHandling;
    int8  EnableSideBorders;

    CString DataFilesDir;
    CString MainDataFilename;
    CString Translation;
    CString GfxFilterID;
    CString GfxDriverID;

    int32 ForceLetterbox;
    CString ReplayFile;

    CEngineSetup();
    void SetDefaults();
    // Read and write binary data
    HErr Read (CStream *in);
    HErr Write(CStream *out);
    // Read and write from/to key-value structure
    HErr ReadCFG (Common::Util::CKeyValueTree *tree);
    HErr WriteCFG(Common::Util::CKeyValueTree *tree);
};

} // namespace Engine
} // namespace AGS

#endif // __AGS_EE_CORE__ENGINESETUP_H
