
#include "Engine/game/agsgame.h"

namespace AGS
{
namespace Engine
{
namespace Game
{

CAGSGame::CAGSGame()
{
}

CAGSGame::~CAGSGame()
{
}

/*const*/ GameState *CAGSGame::GetGameState() /*const*/
{
    return &_play;
}

void CAGSGame::Initialize()
{
    _play.recording      = 0;
    _play.playback       = 0;
    _play.takeover_data  = 0;
}

} // namespace Game
} // namespace Engine
} // namespace AGS
