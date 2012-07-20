
//=============================================================================
//
// AGS Game class
//
// [IKM] 2012-07-11: draft version
//
//=============================================================================
#ifndef __AGS_EE_GAME__AGSGAME_H
#define __AGS_EE_GAME__AGSGAME_H

#include "Engine/ac/gamestate.h"

namespace AGS
{
namespace Engine
{
namespace Game
{

class CAGSGame
{
public:
    CAGSGame();
    ~CAGSGame();

    // TODO: this should be made const
    /*const*/ GameState &GetGameState() /*const*/;

    void            Initialize();

protected:
private:

    // Unfortunately it won't be that easy to get rid of this struct;
    // It is referenced from script directly by address, and Editor
    // wants to have it too.
    GameState   _play;
};

} // namespace Game
} // namespace Engine
} // namespace AGS

#endif // __AGS_EE_GAME__AGSGAME_H
