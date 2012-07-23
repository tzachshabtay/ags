
//=============================================================================
//
// Room class
//
// [IKM] 2012-07-23: draft version
//
//=============================================================================
#ifndef __AGS_EE_GAME__ROOM_H
#define __AGS_EE_GAME__ROOM_H

#include "Common/core/err.h"
#include "Common/ac/roomstruct.h"
#include "Engine/ac/roomstatus.h"

namespace AGS
{
namespace Engine
{
namespace Game
{

// Using-declarations
using AGS::Common::Core::HErr;

class CRoom
{
public:
    CRoom();
    ~CRoom();

    /*const*/ roomstruct    &GetRoomStatic() /*const*/;
    /*const*/ RoomStatus    &GetRoomState() /*const*/;

    HErr                    Initialize();

protected:
private:

    roomstruct  _roomStatic;
    RoomStatus  _roomState;
};

} // namespace Game
} // namespace Engine
} // namespace AGS

#endif // __AGS_EE_GAME__ROOM_H
