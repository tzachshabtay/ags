
#include <string.h>
#include "Common/util/string.h"

namespace AGS
{
namespace Common
{
namespace Util
{

void CString::Append(const CString &s)
{
    // o_0
}

void CString::AppendCStr(const char *sz)
{
    // o_0
}

bool CString::IsEmpty() const
{
    return !s || (strlen(s) == 0);
}

int CString::GetLength() const
{
    return strlen(s);
}

int CString::Compare(const CString &other) const
{
    return 0;
}

void CString::Format(const CString &format, ...)
{
    // o_0
}

int CString::ToInt() const
{
    return 0;
}

} // namespace Util
} // namespace Common
} // namespace AGS