
//=============================================================================
//
// View class
//
// [IKM] 2012-07-27: draft version
//
//=============================================================================
#ifndef __AGS_EE_GAME__VIEW_H
#define __AGS_EE_GAME__VIEW_H

#include "Common/core/err.h"
#include "ac/view.h"
#include "Engine/game/agsdynamicobject.h"

namespace AGS
{
namespace Engine
{
namespace Game
{

// Using-declarations
using AGS::Common::Core::HErr;

class CView : public CAGSDynamicObject
{
public:
    CView();
    ~CView();

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

    /*const*/ ViewStruct    &GetViewStruct() /*const*/;

    HErr                    Read(CStream *in, int32_t game_data_version);
    HErr                    Write(CStream *out, int32_t game_data_version);

    static HErr             ConvertFromView272(const ViewStruct272 &view272, ViewStruct &view);

private:

    ViewStruct  _viewStruct;
};

} // namespace Game
} // namespace Engine
} // namespace AGS

#endif // __AGS_EE_GAME__VIEW_H
