
//=============================================================================
//
// Dynamic objects memory manager
//
//=============================================================================
#ifndef __AGS_EE_GAME__DYNAMICOBJECTSMANAGER_H
#define __AGS_EE_GAME__DYNAMICOBJECTSMANAGER_H

#include "Common/core/err.h"
#include "ac/dynobj/cc_dynamicobject.h"

namespace AGS
{

// Forward declarations
namespace Common
{
namespace Util
{
class CStream;
} // namespace Util
} // namespace Common

namespace Engine
{

// Forward declarations
namespace Util
{
class CManagedObjectPool;
} // namespace Util

namespace Game
{

// Forward declarations
class IDynamicObject;
class IManagedListener;

// Using-declarations
using AGS::Common::Core::HErr;
using AGS::Common::Util::CStream;
using AGS::Engine::Util::CManagedObjectPool;

class CDynamicObjectManager
{
public:
    CDynamicObjectManager();
    ~CDynamicObjectManager();

    // set the class that will be used for dynamic strings
    void        SetStringClassImpl(ICCStringClass *theClass);
    // register a memory handle for the object and allow script
    // pointers to point to it
    refHandle   RegisterManagedObject(IDynamicObject *);
    // register a de-serialized object
    refHandle   RegisterUnserializedObject(int32_t index, IDynamicObject *);
    // unregister a particular object
    int32_t     UnRegisterManagedObject(IDynamicObject *);
    // remove all registered objects
    void        UnregisterAllObjects();
    // serialize all objects to disk
    void        SerializeAllObjects(CStream *out);
    // un-serialise all objects (will remove all currently registered ones)
    int32_t     UnserializeAllObjects(CStream *in, ICCObjectReader *callback);
    // dispose the object if RefCount==0
    void        AttemptDisposeObject(refHandle handle);
    // translate between object handles and memory addresses
    refHandle   GetObjectHandleFromAddress(IDynamicObject *);
    const char  *GetObjectAddressFromHandle(refHandle handle);

    int32_t     AddObjectReference(refHandle handle);
    int32_t     ReleaseObjectReference(refHandle handle);

    HErr        AddObjectListener(IManagedListener *listener);
    void        RemoveObjectListener(IManagedListener *listener);


private:

    CManagedObjectPool &_pool;
};

} // namespace Game
} // namespace Engine
} // namespace AGS

#endif // __AGS_EE_GAME__DYNAMICOBJECTSMANAGER_H
