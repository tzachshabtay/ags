
#include "Engine/game/dynamicobjectmanager.h"
#include "Engine/game/dynamicobject.h"
#include "Engine/game/managedlistener.h"
#include "Engine/util/_managedobjectpool.h"

#include "Common/script/cc_error.h"
#include "Engine/script/script_runtime.h"

namespace AGS
{
namespace Engine
{
namespace Game
{

// Using-declarations
namespace Err = AGS::Common::Core::Err;

CDynamicObjectManager::CDynamicObjectManager()
    : _pool(*new CManagedObjectPool())
{

}

CDynamicObjectManager::~CDynamicObjectManager()
{

}

void CDynamicObjectManager::SetStringClassImpl(ICCStringClass *theClass) {
    stringClassImpl = theClass;
}

refHandle CDynamicObjectManager::RegisterManagedObject(IDynamicObject *object) {
    refHandle handl = _pool.AddObject((const char*)object->GetScriptData(), object);
    ccAddExternalSymbol(const_cast<char*>(object->GetScriptName().GetCStr()), object->GetScriptData());

#ifdef DEBUG_MANAGED_OBJECTS
    char bufff[200];
    sprintf(bufff,"Register managed object type '%s' handle=%d addr=%08X", ((callback == NULL) ? "(unknown)" : callback->GetType()), handl, object);
    write_log(bufff);
#endif

    return handl;
}

// register a de-serialized object
refHandle CDynamicObjectManager::RegisterUnserializedObject(int32_t index, IDynamicObject *object) {
    return _pool.AddObject((const char*)object, object, index);
}

// unregister a particular object
int32_t CDynamicObjectManager::UnRegisterManagedObject(IDynamicObject *object) {
    return _pool.RemoveObject((const char*)object);
}

// remove all registered objects
void CDynamicObjectManager::UnregisterAllObjects() {
    _pool.Reset();
}

// serialize all objects to disk
void CDynamicObjectManager::SerializeAllObjects(CStream *out) {
    _pool.WriteToStream(out);
}

// un-serialise all objects (will remove all currently registered ones)
int32_t CDynamicObjectManager::UnserializeAllObjects(CStream *in, ICCObjectReader *callback) {
    // un-register all existing objects, ready for the un-serialization
    UnregisterAllObjects();
    return _pool.ReadFromStream(in, callback);
}

// dispose the object if RefCount==0
void CDynamicObjectManager::AttemptDisposeObject(refHandle handle) {
    if (_pool.HandleToAddress(handle) != NULL)
        _pool.CheckDispose(handle);
}

// translate between object handles and memory addresses
refHandle CDynamicObjectManager::GetObjectHandleFromAddress(IDynamicObject *object) {
    // set to null
    if (object == NULL)
        return 0;

    refHandle handl = _pool.AddressToHandle((const char*)object);

#ifdef DEBUG_MANAGED_OBJECTS
    char bufff[200];
    sprintf(bufff,"Line %d WritePtr: %08X to %d", currentline, address, handl);
    write_log(bufff);
#endif

    if (handl == 0) {
        cc_error("Pointer cast failure: the object being pointed to is not in the managed object _pool");
        return -1;
    }
    return handl;
}

const char *CDynamicObjectManager::GetObjectAddressFromHandle(refHandle handle) {
    if (handle == 0) {
        return NULL;
    }
    const char *addr = _pool.HandleToAddress(handle);

#ifdef DEBUG_MANAGED_OBJECTS
    char bufff[200];
    sprintf(bufff,"Line %d ReadPtr: %d to %08X", currentline, handle, addr);
    write_log(bufff);
#endif

    if (addr == NULL) {
        cc_error("Error retrieving pointer: invalid handle %d", handle);
        return NULL;
    }
    return addr;
}

int32_t CDynamicObjectManager::AddObjectReference(refHandle handle) {
    if (handle == 0)
        return 0;

    return _pool.AddRef(handle);
}

int32_t CDynamicObjectManager::ReleaseObjectReference(refHandle handle) {
    if (handle == 0)
        return 0;

    if (_pool.HandleToAddress(handle) == NULL) {
        cc_error("Error releasing pointer: invalid handle %d", handle);
        return -1;
    }

    return _pool.SubRef(handle);
}

// TODO!!
HErr CDynamicObjectManager::AddObjectListener(IManagedListener *listener)
{
    return Err::Nil();
}

void CDynamicObjectManager::RemoveObjectListener(IManagedListener *listener)
{

}

} // namespace Game
} // namespace Engine
} // namespace AGS
