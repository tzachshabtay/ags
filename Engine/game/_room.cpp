
#include "util/wgt2allg.h"
#include "Engine/game/_room.h"
#include "Engine/game/dynamicobjectmanager.h"

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

CString CRoom::GetType() const
{
    return "Room";
}

CString CRoom::GetScriptName() const
{
    return "";
}

void *CRoom::GetScriptData() const
{
    return (void*)this;
}

void CRoom::Dispose()
{

}

void CRoom::Serialize(CStream *out) const
{

}

HErr CRoom::Unserialize(CStream *in)
{
    return Err::Nil();
}

/*const*/ roomstruct &CRoom::GetStatic() /*const*/
{
    return _roomStatic;
}

/*const*/ RoomStatus &CRoom::GetState() /*const*/
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
