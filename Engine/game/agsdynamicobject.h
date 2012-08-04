
//=============================================================================
//
// Base AGS managed object
//
//=============================================================================
#ifndef __AGS_EE_GAME__AGSDYNAMICOBJECT_H
#define __AGS_EE_GAME__AGSDYNAMICOBJECT_H

#include "Engine/game/dynamicobject.h"

namespace AGS
{
namespace Engine
{
namespace Game
{

// [IKM] 2012-08-01
// Could be redundant, but will leave it for now;
// if there is any default or common behavior for
// dynamic objects, it should be put here.
class CAGSDynamicObject : public IDynamicObject
{
public:
    virtual ~CAGSDynamicObject(){}

    // Release the object; object should not be used afterwards
    virtual void            Dispose() { delete this; }
};

} // namespace Game
} // namespace Engine
} // namespace AGS

#endif // __AGS_EE_GAME__AGSDYNAMICOBJECT_H
