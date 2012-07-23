
//=============================================================================
//
// AGS Game class
//
// [IKM] 2012-07-11: draft version
//
//=============================================================================
#ifndef __AGS_EE_GAME__AGSGAME_H
#define __AGS_EE_GAME__AGSGAME_H

#include "Common/core/err.h"
#include "Common/util/array.h"
#include "Common/util/ptrarray.h"
#include "Engine/ac/gamestate.h"
#include "ac/lipsync.h"

namespace AGS
{
namespace Engine
{
namespace Game
{

// Forward declarations
class CRoom;

// Using-declarations
using AGS::Common::Core::HErr;
using AGS::Common::Util::CArray;
using AGS::Common::Util::CPtrArray;

class CAGSGame
{
public:
    CAGSGame();
    ~CAGSGame();

    // TODO: this should be made const
    /*const*/ GameState &GetGameState() /*const*/;

    HErr    Initialize();
    void    Shutdown();

protected:

    void    InitGameState();
    HErr    InitRooms();
    HErr    InitSpeech();

private:

    // Unfortunately it won't be that easy to get rid of this struct;
    // It is referenced from script directly by address, and Editor
    // wants to have it too.
    GameState                   _play;
    CPtrArray<CRoom>            _roomStatus;
    CArray<SpeechLipSyncLine>   _splipsync;
};

} // namespace Game
} // namespace Engine
} // namespace AGS

#endif // __AGS_EE_GAME__AGSGAME_H
