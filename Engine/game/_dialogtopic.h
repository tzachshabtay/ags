
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

class CDialogTopic
{
public:
    CDialogTopic();
    ~CDialogTopic();

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
