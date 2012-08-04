
//=============================================================================
//
// A close-to-original reimplementation of ManagedObjectPool struct
//
//=============================================================================
#ifndef __AGS_EE_UTIL__MANAGEDOBJECTSPOOL_H
#define __AGS_EE_UTIL__MANAGEDOBJECTSPOOL_H

#include "Common/core/types.h"
#include "Common/util/array.h"
#include "ac/dynobj/cc_dynamicobject.h"

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

// Forward declarations
namespace Game
{
class IDynamicObject;
}

namespace Util
{

// Using-declarations
using AGS::Common::Util::CArray;
using AGS::Common::Util::CStream;
using AGS::Engine::Game::IDynamicObject;

class CManagedObjectPool {
public:
    CManagedObjectPool();
    ~CManagedObjectPool();

    refHandle   AddRef(refHandle handle);
    int32_t     CheckDispose(refHandle handle);
    refHandle   SubRef(refHandle handle);
    refHandle   AddressToHandle(const char *addr);
    const char* HandleToAddress(refHandle handle);
    int32_t     RemoveObject(const char *address);
    void        RunGarbageCollectionIfAppropriate();
    void        RunGarbageCollection();
    int32_t     AddObject(const char *address, IDynamicObject *callback, int32_t useSlot = -1);
    void        WriteToStream(CStream *out);
    int32_t     ReadFromStream(CStream *in, ICCObjectReader *reader);
    void        Reset();

    static const int32_t    ArrayAllocSize;
    const char*             DisableDisposeForObject;

private:
    struct ManagedObject {
        refHandle       Handle;
        const char      *Addr;
        IDynamicObject  *Callback;
        int32_t         RefCount;

        void    Init(refHandle the_handle, const char *the_address, IDynamicObject *the_callback);
        int32_t Remove(bool force);
        int32_t AddRef();
        int32_t CheckDispose();
        int32_t SubRef();
        void    SubRefNoDispose();
    };

    // TODO: perhaps binary tree (Map) would be faster
    CArray<ManagedObject>     _objects;
    // FIXME: These two are probably redundant since we are using CArray
    int32_t                 _arrayAllocLimit;
    int32_t                 _numObjects;  // not actually numObjects, but the highest index used

    int32_t                 _objectCreationCounter;  // used to do garbage collection every so often
};

} // namespace Util
} // namespace Engine
} // namespace AGS

#endif // __AGS_EE_UTIL__MANAGEDOBJECTSPOOL_H
