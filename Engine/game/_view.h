
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

namespace AGS
{
namespace Engine
{
namespace Game
{

// Using-declarations
using AGS::Common::Core::HErr;

class CView
{
public:
    CView();
    ~CView();

    /*const*/ ViewStruct    &GetViewStruct() /*const*/;

    HErr                    Read(CStream *in, int32_t game_data_version);
    HErr                    Write(CStream *out, int32_t game_data_version);

    static HErr             ConvertFromView272(const ViewStruct272 &view272, ViewStruct &view);

protected:
private:

    ViewStruct  _viewStruct;
};

} // namespace Game
} // namespace Engine
} // namespace AGS

#endif // __AGS_EE_GAME__VIEW_H
