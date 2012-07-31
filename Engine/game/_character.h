
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

namespace AGS
{
namespace Engine
{
namespace Game
{

// Using-declarations
using AGS::Common::Core::HErr;

class CCharacter
{
public:
    CCharacter();
    ~CCharacter();

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
