
#include "util/wgt2allg.h"
#include "Common/core/out.h"
#include "Common/util/stream.h"
#include "Engine/core/agsengine.h"
#include "Engine/core/engine_setup.h"
#include "Engine/game/agsgame.h"
#include "Engine/game/assets_manager.h"
#include "Engine/game/_room.h"
#include "Engine/platform/base/agsplatformdriver.h"

namespace AGS
{
namespace Engine
{
namespace Game
{

// Using-declarations
using AGS::Engine::Core::CAGSEngine;
using AGS::Engine::Core::CEngineSetup;
namespace Out = AGS::Common::Core::Out;
namespace Err = AGS::Common::Core::Err;

CAGSGame::CAGSGame()
{
}

CAGSGame::~CAGSGame()
{
}

/*const*/ GameState &CAGSGame::GetGameState() /*const*/
{
    return _play;
}

HErr CAGSGame::Initialize()
{
    InitGameState();

    HErr err = InitRooms();
    if (!err.IsNil())
    {
        return err;
    }

    err = InitSpeech();
    if (!err.IsNil())
    {
        return err;
    }



    return Err::Nil();
}

void CAGSGame::Shutdown()
{

}

void CAGSGame::InitGameState()
{
    // init general state
    _play.recording      = 0;
    _play.playback       = 0;
    _play.takeover_data  = 0;

    InitRooms();
    InitSpeech();
}

HErr CAGSGame::InitRooms()
{
    Out::Notify("Initializing rooms");
    // Not needed here since using dynamic arrays
    /*
    roomstats=(RoomStatus*)calloc(sizeof(RoomStatus),MAX_ROOMS);
    for (int ee=0;ee<MAX_ROOMS;ee++) {
        
    }
    */

    return Err::Nil();
}

HErr CAGSGame::InitSpeech()
{
    _play.want_speech=-2;

    CEngineSetup &setup = CAGSEngine::GetSetup();
    if (setup.NoSpeechPack != 0)
    {
        return Err::Nil();
    }

    AGSPlatformDriver *platform = AGSPlatformDriver::GetDriver();
    
    CAssetsManager *assets      = CAGSEngine::GetAssetsManager();

    CStream *speechsync = assets->OpenAsset(assets->GetSpeechFileName(), "syncdata.dat", "rb");
    if (speechsync != NULL) {
        // this game has voice lip sync
        if (speechsync->ReadInt32() != 4)
        { 
            // Don't display this warning.
            // platform->DisplayAlert("Unknown speech lip sync format (might be from older or newer version); lip sync disabled");
        }
        else {
            int num_lip_lines = speechsync->ReadInt32();
            for (int i = 0; i < num_lip_lines; ++i)
            {
                _splipsync[i].Read(speechsync);
            }
        }
        delete speechsync;
    }
    _play.want_speech=1;
        //}
    //}

    return Err::Nil();
}

} // namespace Game
} // namespace Engine
} // namespace AGS
