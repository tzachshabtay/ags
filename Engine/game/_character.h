
//=============================================================================
//
// Character class
//
// [IKM] 2012-07-27: draft version
//
//=============================================================================
#ifndef __AGS_EE_GAME__CHARACTER_H
#define __AGS_EE_GAME__CHARACTER_H

#include "Common/core/err.h"
#include "Common/ac/characterinfo.h"
#include "Engine/ac/charactercache.h"
#include "Engine/ac/characterextras.h"
#include "Engine/game/agsdynamicobject.h"

namespace AGS
{
namespace Engine
{
namespace Game
{

// Using-declarations
using AGS::Common::Core::HErr;

class CCharacter : public CAGSDynamicObject
{
public:
    CCharacter();
    virtual ~CCharacter();

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

    /*const*/ CharacterInfo     &GetInfo() /*const*/;
    /*const*/ CharacterExtras   &GetExtras() /*const*/;
    /*const*/ CharacterCache    &GetCache() /*const*/;

private:

    CharacterInfo   _charinfo;
    CharacterExtras _charextras;
    CharacterCache  _cache;
};

} // namespace Game
} // namespace Engine
} // namespace AGS

#endif // __AGS_EE_GAME__CHARACTER_H
