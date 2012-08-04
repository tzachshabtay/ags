
//=============================================================================
//
// Helper handles for dynamic objects
//
//=============================================================================
#ifndef __AGS_EE_GAME__MANAGEDLISTENER_H
#define __AGS_EE_GAME__MANAGEDLISTENER_H

namespace AGS
{
namespace Engine
{
namespace Game
{

class IManagedListener
{
public:
    virtual ~IManagedListener(){}

    virtual refHandle   GetHandle() const   = 0;
    virtual bool        IsNull() const      = 0;
    virtual void        OnObjectDisposed()  = 0;
};

//=============================================================================
//
// Class CClientRef, listens to CDynamicObject state and is notified when
// it's disposed.
//

template <typename TClass> class CClientRef : public IManagedListener
{
public:
    CClientRef(refHandle handle)
        : Handle(handle)
    {
        CAGSEngine::GetGame()->GetObjectManager()->AddObjectListener(this);
    }
    ~CClientRef()
    {
        CAGSEngine::GetGame()->GetObjectManager()->RemoveObjectListener(this);
    }

    virtual refHandle   GetHandle() const
    {
        return Handle;
    }

    virtual bool        IsNull() const
    {
        return Handle == 0;
    }

    virtual void        OnObjectDisposed()
    {
        Handle = 0;
        Obj = NULL;
    }

    virtual TClass *    GetObject() const
    {
        return Obj;
    }

    TClass *operator ->()
    {
        return Obj;
    }

private:
    TClass    *Obj;
    refHandle Handle;
};

//=============================================================================
//
// Class COwnerRef, inherits CClientRef functionality, and also works as an
// auto-reference: increases ref count when created and decreases when
// destroyed.
//

template <typename TClass> class COwnerRef : public CClientRef<TClass>
{
public:
    COwnerRef(refHandle handle)
        : CClientRef(handle)
    {
        CAGSEngine::GetGame()->GetObjectManager()->AddObjectReference(Handle);
    }
    ~COwnerRef()
    {
        CAGSEngine::GetGame()->GetObjectManager()->ReleaseObjectReference(Handle);
    }
};

} // namespace Game
} // namespace Engine
} // namespace AGS

#endif // __AGS_EE_GAME__MANAGEDLISTENER_H
