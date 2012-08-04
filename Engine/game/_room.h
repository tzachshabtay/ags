
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
#include "Engine/game/agsdynamicobject.h"

namespace AGS
{
namespace Engine
{
namespace Game
{

// Using-declarations
using AGS::Common::Core::HErr;

class CRoom : public CAGSDynamicObject
{
public:
    CRoom();
    ~CRoom();

    // Returns internal type name
    virtual CString         GetType() const;
    // Returns entity's script name
    virtual CString         GetScriptName() const;
    // Returns a memory address that should be registered as a script object pointer
    // in memory management system
    virtual void            *GetScriptData() const;
    // Release the object; object should not be used afterwards
    virtual void            Dispose();
    // Write the object into stream
    virtual void            Serialize(CStream *out) const;
    // Read the object from stream
    virtual HErr            Unserialize(CStream *in);

    /*const*/ roomstruct    &GetStatic() /*const*/;
    /*const*/ RoomStatus    &GetState() /*const*/;

    HErr                    Initialize();

private:

    roomstruct  _roomStatic;
    RoomStatus  _roomState;
};

} // namespace Game
} // namespace Engine
} // namespace AGS

#endif // __AGS_EE_GAME__ROOM_H
