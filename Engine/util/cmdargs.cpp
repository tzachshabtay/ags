
#include "Engine/platform/base/agsplatformdriver.h"
#include "Engine/util/cmdargs.h"
#ifdef WINDOWS_VERSION
#include <shlwapi.h>
#endif

namespace AGS
{
namespace Engine
{
namespace Util
{

// Using-declarations
using AGS::Common::Core::CErrorHandle;
namespace Err = AGS::Common::Core::Err;

CCmdArgs::CCmdArgs()
{
}

HErr CCmdArgs::ParseCmdLine(int argc, const char * const argv[])
{
    // this code is copied from main()
#ifdef WINDOWS_VERSION
    int wArgc;
    LPWSTR *wArgv;
    wArgv = CommandLineToArgvW(GetCommandLineW(), &wArgc);
    if (wArgv == NULL)
    {
        AGSPlatformDriver::GetDriver()->DisplayAlert("CommandLineToArgvW failed, unable to start the game.");
        return Err::FromCode(9);
    }
#else
    //global_argv = argv;
#endif
    return Err::Nil();
}

void CCmdArgs::Clear()
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

CString CCmdArgs::operator[](int index) const
{
    return GetArgStringAt(index);
}

} // namespace Util
} // namespace Engine
} // namespace AGS
