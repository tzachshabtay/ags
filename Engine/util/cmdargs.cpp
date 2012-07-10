
#include "Engine/util/cmdargs.h"

namespace AGS
{
namespace Engine
{
namespace Util
{

CCmdArgs::CCmdArgs()
{
}

CCmdArgs::CCmdArgs(int argc, const char * const argv[])
{
}

int CCmdArgs::GetCount() const
{
    return 0;
}

CString CCmdArgs::GetArgStringAt(int index) const
{
    return "";
}

CString CCmdArgs::GetArgKeyAt(int index) const
{
    return "";
}

CString CCmdArgs::GetArgValueAt(int index) const
{
    return "";
}

bool CCmdArgs::ContainsArgString(const CString &exact_string) const
{
    return true;
}

bool CCmdArgs::ContainsArgKey(const CString &exact_name) const
{
    return true;
}

bool CCmdArgs::ContainsArgKey(const CString &short_name, const CString &long_name) const
{
    return true;
}

CString CCmdArgs::GetArgValue(const CString &exact_name) const
{
    return "";
}

CString CCmdArgs::GetArgValue(const CString &short_name, const CString &long_name) const
{
    return "";
}

} // namespace Util
} // namespace Engine
} // namespace AGS
