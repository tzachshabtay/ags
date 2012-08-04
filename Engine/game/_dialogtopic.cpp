
#include "Engine/game/_dialogtopic.h"
#include "Engine/game/dynamicobjectmanager.h"

namespace AGS
{
namespace Engine
{
namespace Game
{

namespace Err = AGS::Common::Core::Err;

CDialogTopic::CDialogTopic()
{
}

CDialogTopic::~CDialogTopic()
{

}

CString CDialogTopic::GetType() const
{
    return "DialogTopic";
}

CString CDialogTopic::GetScriptName() const
{
    return "";
}

void *CDialogTopic::GetScriptData() const
{
    return (void*)&_dialogTopic;
}

void CDialogTopic::Dispose()
{

}

void CDialogTopic::Serialize(CStream *out) const
{

}

HErr CDialogTopic::Unserialize(CStream *in)
{
    return Err::Nil();
}

/*const*/ DialogTopic &CDialogTopic::GetDialogTopic() /*const*/
{
    return _dialogTopic;
}

HErr CDialogTopic::Read(CStream *in)
{
    return Err::Nil();
}

HErr CDialogTopic::Write(CStream *out)
{
    return Err::Nil();
}

} // namespace Game
} // namespace Engine
} // namespace AGS
