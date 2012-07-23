
#include "util/wgt2allg.h"
#include "Engine/game/_room.h"

namespace AGS
{
namespace Engine
{
namespace Game
{

// Using-declarations
namespace Err = AGS::Common::Core::Err;

CRoom::CRoom()
{

}

CRoom::~CRoom()
{

}

/*const*/ roomstruct &CRoom::GetRoomStatic() /*const*/
{
    return _roomStatic;
}

/*const*/ RoomStatus &CRoom::GetRoomState() /*const*/
{
    return _roomState;
}

HErr CRoom::Initialize()
{
    _roomState.beenhere=0;
    _roomState.numobj=0;
    _roomState.tsdatasize=0;
    _roomState.tsdata=NULL;

    return Err::Nil();
}

} // namespace Game
} // namespace Engine
} // namespace AGS
