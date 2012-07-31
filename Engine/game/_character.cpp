
#include "util/wgt2allg.h"
#include "Engine/game/_character.h"

namespace AGS
{
namespace Engine
{
namespace Game
{

CCharacter::CCharacter()
{

}

CCharacter::~CCharacter()
{

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
