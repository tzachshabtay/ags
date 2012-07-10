
//=============================================================================
//
// [IKM] 2012-07-10: cmdargs helper draft/placeholder
//
//=============================================================================
#ifndef __AGS_EE_UTILS__CMDARGS_H
#define __AGS_EE_UTILS__CMDARGS_H

#include "Common/util/string.h"

namespace AGS
{
namespace Engine
{
namespace Util
{

using AGS::Common::Util::CString;

//
// The class is supposed to parse command line and store arguments, while ignoring any duplicates;
// On terminology:
// For example we have an argument "-cfg=C:\\Program~1\\MyGame\\whatever.cfg"
// - ArgString - is exactly argument substring between spaces, i.e "-cfg=C:\\Program~1\\MyGame\\whatever.cfg";
// - ArgKey    - is argument substring before '=' symbol, i.e "cfg";
// - ArgValue  - is argument substring after '=' symbol, i.e "C:\\Program~1\\MyGame\\whatever.cfg"
//
class CCmdArgs
{
public:
    CCmdArgs();
    CCmdArgs(int argc, const char * const argv[]);

    int     GetCount() const;

    CString GetArgStringAt(int index) const;
    CString GetArgKeyAt(int index) const;
    CString GetArgValueAt(int index) const;

    bool ContainsArgString(const CString &exact_string) const;
    // GNU comformant programs use '-' preceded short args and '--' preceded long name aliases;
    // AGS does not seem to follow this convention, however it is nice to have an opportunity to
    // check both possible aliases at once.
    bool ContainsArgKey(const CString &exact_name) const;
    bool ContainsArgKey(const CString &short_name, const CString &long_name) const;

    CString GetArgValue(const CString &exact_name) const;
    CString GetArgValue(const CString &short_name, const CString &long_name) const;
};

} // namespace Util
} // namespace Engine
} // namespace AGS

#endif // __AGS_EE_UTILS__CMDARGS_H
