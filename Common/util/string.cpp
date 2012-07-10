
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

void CString::AppendSz(const char *sz)
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

} // namespace Util
} // namespace Common
} // namespace AGS