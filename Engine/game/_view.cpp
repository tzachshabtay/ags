
#include "util/wgt2allg.h"
#include "Engine/game/agsgame.h"
#include "Engine/game/_view.h"

namespace AGS
{
namespace Engine
{
namespace Game
{

// Using-declarations
namespace Err = AGS::Common::Core::Err;

CView::CView()
{

}

CView::~CView()
{

}

/*const*/ ViewStruct &CView::GetViewStruct() /*const*/
{
    return _viewStruct;
}

HErr CView::Read(CStream *in, int32_t game_data_version)
{
    HErr err; 
    if (game_data_version > kGameData_272) // 3.x views
    {
        err = _viewStruct.Read(in);
        if (!err.IsNil())
        {
            return err;
        }
    }
    else // 2.x views
    {
        ViewStruct272 old_view;
        err = old_view.Read(in);
        if (!err.IsNil())
        {
            return err;
        }
        err = ConvertFromView272(old_view, _viewStruct);
        if (!err.IsNil())
        {
            return err;
        }
    }

    return Err::Nil();
}

HErr CView::Write(CStream *out, int32_t game_data_version)
{
    return Err::Nil();
}

/*static*/ HErr CView::ConvertFromView272(const ViewStruct272 &view272, ViewStruct &view)
{
    return Err::Nil();
}

} // namespace Game
} // namespace Engine
} // namespace AGS
