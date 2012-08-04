
//=============================================================================
//
// Dialog topic class
//
// [IKM] 2012-07-27: draft version
//
//=============================================================================
#ifndef __AGS_EE_GAME__DIALOGTOPIC_H
#define __AGS_EE_GAME__DIALOGTOPIC_H

#include "Common/core/err.h"
#include "ac/dialogtopic.h"
#include "Engine/game/agsdynamicobject.h"

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

class CDialogTopic : public CAGSDynamicObject
{
public:
    CDialogTopic();
    ~CDialogTopic();

    // Returns internal type name
    virtual CString         GetType() const;
    // Returns entity's script name
    virtual CString         GetScriptName() const;
    // Returns a memory address that should be registered as a script object pointer
    // in memory management system
    virtual void            *GetScriptData() const;
    // Release the object; object should not be used afterwards
    virtual void            Dispose();
    // Write the object into stream
    virtual void            Serialize(CStream *out) const;
    // Read the object from stream
    virtual HErr            Unserialize(CStream *in);

    /*const*/ DialogTopic   &GetDialogTopic() /*const*/;

    HErr    Read(CStream *in);
    HErr    Write(CStream *out);

private:

    DialogTopic   _dialogTopic;
};

} // namespace Game
} // namespace Engine
} // namespace AGS

#endif // __AGS_EE_GAME__DIALOGTOPIC_H
