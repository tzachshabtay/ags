
//=============================================================================
//
// Managed object interface
//
//-----------------------------------------------------------------------------
//
// There should be a distinction between actual object in memory and its in-
// game representation; we may refer the last as 'entity'.
//
// For the reference-counted object there's always at least one reference held
// either by engine or script system.
// Engine's internal references are released when the entity, which this object
// represents, is no longer considered needed or existing. Script reference is
// released as soon as it is nullified (a null reference is assigned to it) or
// destroyed when script execution exits the variable's life scope.
//
// Script has its ways to access every object which was created in the engine
// and registered in the memory management system (and not yet disposed).
//
// Consider following two situations:
// 1) Game dev orders to create temporary text overlay in script function and
// stores pointer for future use. When the overlay time is depleted, it is
// removed from screen and considered obsolete, but since the script pointer
// is stored, the object should not be deleted right away to allow safe
// referencing from the script. Only after it is explicitly nullified, the
// object is disposed and now may be physically removed from memory at any
// time by the garbage collector.
// 2) Game dev orders to create temporary text overlay, but does not store
// the pointer. As soon as script function is exited, the unnamed pointer
// is destroyed and object becomes unreferenced from script. However, there's
// still an existing overlay entity which must be displayed for some time and
// that keeps object in memory. As soon as overlay lifetime runs out, the
// object is disposed.
//
//-----------------------------------------------------------------------------
//
// This would be natural to change original AGS behavior that differentiated
// a script object, a manager-object, and game data object (entity) and unify
// those three concepts in one class. Unfortunately we have to pay a debt to
// backwards compatibility.
// Therefore the new system will work in following way:
// The new CDynamicObjectManager class now works with IDynamicObject managed
// objects. Registration combines both registering managed object and adding
// export symbol. IDynamicObject is assigned a role of "callback" but also is
// asked to provide an address of actual data. For original entity types that
// data has to be identical to AGS 3.2 entity data so that if the engine runs
// older games they could safely access data by address.
//
//=============================================================================
#ifndef __AGS_EE_GAME__DYNAMICOBJECT_H
#define __AGS_EE_GAME__DYNAMICOBJECT_H

#include "Common/core/err.h"

namespace AGS
{
// Forward declarations
namespace Common
{
namespace Util
{
class CStream;
}
}

namespace Engine
{
namespace Game
{

// Using-declarations
using AGS::Common::Core::HErr;
using AGS::Common::Util::CStream;
using AGS::Common::Util::CString;

class IDynamicObject
{
public:
    virtual ~IDynamicObject(){}

    // Returns internal type name
    virtual CString         GetType() const = 0;
    // Returns entity's script name
    virtual CString         GetScriptName() const = 0;
    // Returns a memory address that should be registered as a script object pointer
    // in memory management system
    virtual void            *GetScriptData() const = 0;
    // Release the object; object should not be used afterwards
    virtual void            Dispose() = 0;
    // Write the object into stream
    virtual void            Serialize(CStream *out) const = 0;
    // Read the object from stream
    virtual HErr            Unserialize(CStream *in) = 0;
};

} // namespace Game
} // namespace Engine
} // namespace AGS

#endif // __AGS_EE_GAME__DYNAMICOBJECT_H
