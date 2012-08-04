
#include "util/wgt2allg.h"
#include "Engine/game/_character.h"
#include "Engine/game/dynamicobjectmanager.h"

namespace AGS
{
namespace Engine
{
namespace Game
{

// Using-declarations
namespace Err = AGS::Common::Core::Err;

CCharacter::CCharacter()
{

}

CCharacter::~CCharacter()
{

}

CString CCharacter::GetType() const
{
    return "Character";
}

CString CCharacter::GetScriptName() const
{
    return _charinfo.scrname;
}

void *CCharacter::GetScriptData() const
{
    return (void*)&_charinfo;
}

void CCharacter::Dispose()
{

}

void CCharacter::Serialize(CStream *out) const
{

}

HErr CCharacter::Unserialize(CStream *in)
{
    return Err::Nil();
}

/*const*/ CharacterInfo &CCharacter::GetInfo() /*const*/
{
    return _charinfo;
}

/*const*/ CharacterExtras &CCharacter::GetExtras() /*const*/
{
    return _charextras;
}

/*const*/ CharacterCache &CCharacter::GetCache() /*const*/
{
    return _cache;
}

} // namespace Game
} // namespace Engine
} // namespace AGS
