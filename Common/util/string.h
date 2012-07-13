
//=============================================================================
//
// [IKM] 2012-07-09: placeholder for the string class
//
//=============================================================================
#ifndef __AGS_CN_UTILS__STRING_H
#define __AGS_CN_UTILS__STRING_H

#include "Common/core/types.h"

namespace AGS
{
namespace Common
{
namespace Util
{

struct CString
{
    char * s;

    CString()
    {
        s = NULL;
    }

    CString(const char *sz)
    {
        s = const_cast<char*>(sz);
    }

    //
    // Please remember this is a draft/placeholder; the interface may change in future
    //

    inline const char *GetCStr() const { return s; }

    // Appends CString contents
    void Append(const CString &s);
    // Appends a null-terminated string
    void AppendCStr(const char *sz);
    // Checks if string is empty or null
    bool IsEmpty() const;
    int  GetLength() const;
    int  Compare(const CString &other) const;
    void Format(const CString &format, ...);

    char &operator[](int c) { return s[c]; }

    int ToInt() const;
};

} // namespace Util
} // namespace Common
} // namespace AGS

#endif // __AGS_CN_UTILS__STRING_H
