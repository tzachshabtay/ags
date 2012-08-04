
#include "Engine/util/_managedobjectpool.h"
#include "Engine/game/dynamicobject.h"

#define OBJECT_CACHE_MAGIC_NUMBER 0xa30b
#define SERIALIZE_BUFFER_SIZE 10240
const int ARRAY_INCREMENT_SIZE = 100;
const int GARBAGE_COLLECTION_INTERVAL = 100;

namespace AGS
{
namespace Engine
{
namespace Util
{

CManagedObjectPool::CManagedObjectPool() {
    _numObjects = 1;
    _arrayAllocLimit = 10;
    _objects.SetCount(_arrayAllocLimit);
    DisableDisposeForObject = NULL;
}

CManagedObjectPool::~CManagedObjectPool()
{

}

refHandle CManagedObjectPool::AddRef(refHandle Handle) {
    return _objects[Handle].AddRef();
}

int32_t CManagedObjectPool::CheckDispose(refHandle Handle) {
    return _objects[Handle].CheckDispose();
}

refHandle CManagedObjectPool::SubRef(refHandle Handle) {
    if ((DisableDisposeForObject != NULL) && 
        (_objects[Handle].Addr == DisableDisposeForObject))
        _objects[Handle].SubRefNoDispose();
    else
        _objects[Handle].SubRef();
    return _objects[Handle].RefCount;
}

refHandle CManagedObjectPool::AddressToHandle(const char *Addr) {
    // this function is only called when a pointer is set
    // SLOW LOOP ALERT, improve at some point
    for (int32_t kk = 1; kk < _arrayAllocLimit; kk++) {
        if (_objects[kk].Addr == Addr)
            return _objects[kk].Handle;
    }
    return 0;
}

const char* CManagedObjectPool::HandleToAddress(refHandle Handle) {
    // this function is called often (whenever a pointer is used)
    if ((Handle < 1) || (Handle >= _arrayAllocLimit))
        return NULL;
    if (_objects[Handle].Handle == 0)
        return NULL;
    return _objects[Handle].Addr;
}

int32_t CManagedObjectPool::RemoveObject(const char *address) {
    refHandle handl = AddressToHandle(address);
    if (handl == 0)
        return 0;

    _objects[handl].Remove(true);
    return 1;
}

void CManagedObjectPool::RunGarbageCollectionIfAppropriate()
{
    if (_objectCreationCounter > GARBAGE_COLLECTION_INTERVAL)
    {
        _objectCreationCounter = 0;
        RunGarbageCollection();
    }
}

void CManagedObjectPool::RunGarbageCollection()
{
    //write_log("Running garbage collection");

    for (int32_t i = 1; i < _numObjects; i++) 
    {
        if ((_objects[i].RefCount < 1) && (_objects[i].Callback != NULL)) 
        {
            _objects[i].Remove(false);
        }
    }
}

int32_t CManagedObjectPool::AddObject(const char *address, IDynamicObject *Callback, int32_t useSlot) {
    if (useSlot == -1)
        useSlot = _numObjects;

    _objectCreationCounter++;

    if (useSlot < _arrayAllocLimit) {
        // still space in the array, so use it
        _objects[useSlot].Init(useSlot, address, Callback);
        if (useSlot == _numObjects)
            _numObjects++;
        return useSlot;
    }
    else {
        // array has been used up
        if (useSlot == _numObjects) {
            // if adding new (not un-serializing) check for empty slot
            // check backwards, since newer _objects don't tend to last
            // refHandle
            for (int32_t i = _arrayAllocLimit - 1; i >= 1; i--) {
                if (_objects[i].Handle == 0) {
                    _objects[i].Init(i, address, Callback);
                    return i;
                }
            }
        }
        // no empty slots, expand array
        while (useSlot >= _arrayAllocLimit)
            _arrayAllocLimit += ARRAY_INCREMENT_SIZE;

        _objects.SetCount(_arrayAllocLimit);
        // CHECKME
        //memset(&_objects[useSlot], 0, sizeof(ManagedObject) * ARRAY_INCREMENT_SIZE);
        _objects[useSlot].Init(useSlot, address, Callback);
        if (useSlot == _numObjects)
            _numObjects++;
        return useSlot;
    }
}

void CManagedObjectPool::WriteToStream(CStream *out) {
    // TODO!
    /*
    int32_t serializeBufferSize = SERIALIZE_BUFFER_SIZE;
    char *serializeBuffer = (char*)malloc(serializeBufferSize);

    putw(OBJECT_CACHE_MAGIC_NUMBER, output);
    putw(1, output);  // version
    putw(_numObjects, output);

    // use this opportunity to clean up any non-referenced pointers
    RunGarbageCollection();

    for (int32_t i = 1; i < _numObjects; i++) 
    {
        if ((_objects[i].Handle) && (_objects[i].Callback != NULL)) {
            // write the type of the object
            fputstring((char*)_objects[i].Callback->GetType(), output);
            // now write the object data
            int32_t bytesWritten = _objects[i].Callback->Serialize(_objects[i].Addr, serializeBuffer, serializeBufferSize);
            if ((bytesWritten < 0) && ((-bytesWritten) > serializeBufferSize))
            {
                // buffer not big enough, re-allocate with requested size
                serializeBufferSize = -bytesWritten;
                serializeBuffer = (char*)realloc(serializeBuffer, serializeBufferSize);
                bytesWritten = _objects[i].Callback->Serialize(_objects[i].Addr, serializeBuffer, serializeBufferSize);
            }
            putw(bytesWritten, output);
            if (bytesWritten > 0)
                fwrite(serializeBuffer, bytesWritten, 1, output);
            putw(_objects[i].RefCount, output);
        }
        else  // write empty string if we cannot serialize it
            fputc(0, output); 
    }

    free(serializeBuffer);
    */
}

int32_t CManagedObjectPool::ReadFromStream(CStream *in, ICCObjectReader *reader) {
    // TODO!
    /*
    int32_t serializeBufferSize = SERIALIZE_BUFFER_SIZE;
    char *serializeBuffer = (char*)malloc(serializeBufferSize);
    char typeNameBuffer[200];

    if (getw(input) != OBJECT_CACHE_MAGIC_NUMBER) {
        cc_error("Data was not written by ccSeralize");
        return -1;
    }

    if (getw(input) != 1) {
        cc_error("Invalid data version");
        return -1;
    }

    int32_t numObjs = getw(input);

    if (numObjs >= _arrayAllocLimit) {
        _arrayAllocLimit = numObjs + ARRAY_INCREMENT_SIZE;
        free(_objects);
        _objects = (ManagedObject*)calloc(sizeof(ManagedObject), _arrayAllocLimit);
    }
    _numObjects = numObjs;

    for (int32_t i = 1; i < numObjs; i++) {
        fgetstring_limit(typeNameBuffer, input, 199);
        if (typeNameBuffer[0] != 0) {
            int32_t numBytes = getw(input);
            if (numBytes > serializeBufferSize) {
                serializeBufferSize = numBytes;
                serializeBuffer = (char*)realloc(serializeBuffer, serializeBufferSize);
            }
            if (numBytes > 0)
                fread(serializeBuffer, numBytes, 1, input);

            if (strcmp(typeNameBuffer, CC_DYNAMIC_ARRAY_TYPE_NAME) == 0)
            {
                globalDynamicArray.Unserialize(i, serializeBuffer, numBytes);
            }
            else
            {
                reader->Unserialize(i, typeNameBuffer, serializeBuffer, numBytes);
            }
            _objects[i].RefCount = getw(input);
        }
    }

    free(serializeBuffer);
    */
    return 0;
}

void CManagedObjectPool::Reset() {
    // de-allocate all _objects
    for (int32_t kk = 1; kk < _arrayAllocLimit; kk++) {
        if (_objects[kk].Handle)
            _objects[kk].Remove(true);
    }
    // CHECKME
    //memset(&_objects[0], 0, sizeof(ManagedObject) * _arrayAllocLimit);
    _numObjects = 1;
}

void CManagedObjectPool::ManagedObject::Init(refHandle theHandle, const char *theAddress, IDynamicObject *theCallback) {
    Handle = theHandle;
    Addr = theAddress;
    Callback = theCallback;
    RefCount = 0;

#ifdef DEBUG_MANAGED_OBJECTS
    char bufff[200];
    sprintf(bufff,"Allocated managed object Handle=%d, type=%s", theHandle, theCallback->GetType());
    write_log(bufff);
#endif
}

int32_t CManagedObjectPool::ManagedObject::Remove(bool force) {

    // CHECKME
    /*
    if ((Callback != NULL) && (Callback->Dispose(Addr, force) == 0) &&
        (force == false))
        return 0;
        */
    if (!Callback)
    {
        return 0;
    }

    Callback->Dispose();

#ifdef DEBUG_MANAGED_OBJECTS
    char bufff[200];
    sprintf(bufff,"Line %d Disposing managed object Handle=%d", currentline, Handle);
    write_log(bufff);
#endif

    Handle = 0;
    Addr = 0;
    Callback = NULL;
    return 1;
}

int32_t CManagedObjectPool::ManagedObject::AddRef() {

#ifdef DEBUG_MANAGED_OBJECTS
    char bufff[200];
    sprintf(bufff,"Line %d AddRef: Handle=%d new refcount=%d", currentline, Handle, RefCount+1);
    write_log(bufff);
#endif

    return ++RefCount;
}

int32_t CManagedObjectPool::ManagedObject::CheckDispose() {
    if ((RefCount < 1) && (Callback != NULL)) {
        if (Remove(false))
            return 1;
    }
    return 0;
}

int32_t CManagedObjectPool::ManagedObject::SubRef() {
    RefCount--;

#ifdef DEBUG_MANAGED_OBJECTS
    char bufff[200];
    sprintf(bufff,"Line %d SubRef: Handle=%d new refcount=%d", currentline, Handle, RefCount);
    write_log(bufff);
#endif

    return CheckDispose();
}

void CManagedObjectPool::ManagedObject::SubRefNoDispose() {
    RefCount--;

#ifdef DEBUG_MANAGED_OBJECTS
    char bufff[200];
    sprintf(bufff,"Line %d SubRefNoDispose: Handle=%d new refcount=%d", currentline, Handle, RefCount);
    write_log(bufff);
#endif
}

} // namespace Util
} // namespace Engine
} // namespace AGS
