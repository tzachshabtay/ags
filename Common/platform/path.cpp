
#include "Common/platform/path.h"
#include "Common/platform/platform_api.h"

namespace AGS
{
namespace Common
{
namespace Platform
{

// Using-declarations
namespace Err = AGS::Common::Core::Err;

namespace Path
{

bool IsPath(const CString &file_name)
{
    return false;
}

CString FixFileName(const CString &file_name)
{
    char *sz_buf = file_name.s;

    fix_filename_case(sz_buf);
    fix_filename_slashes(sz_buf);

    return CString(sz_buf); // duh, won't work, need normal code
}

CString GetFileName(const CString &file_name)
{
    return get_filename(file_name.GetCStr());
}

CString GetParentPath(const CString &file_name)
{
    return "";
}

CString GetShortPath(const CString &path)
{
#if defined WINDOWS_VERSION
    char *short_path;
    LPCWSTR dataFilePath = NULL; // = path
    WCHAR directoryPathBuffer[MAX_PATH];
    if (GetShortPathNameW(dataFilePath, directoryPathBuffer, MAX_PATH) == 0)
    {
        return "";
    }
    short_path = (char*)malloc(MAX_PATH);
    WideCharToMultiByte(CP_ACP, 0, directoryPathBuffer, -1, short_path, MAX_PATH, NULL, NULL);
    return short_path;
#else
    // ?
    return "";
#endif
}

CString MakePath(const CString &parent_path, const CString &child_path)
{
    return "";
}

CString GetCurrentDirectory()
{
    char sz_buf[256];
    _getcwd (sz_buf, 255);

    return CString(sz_buf); // duh, won't work, need normal code
}

HErr SetCurrentDirectory(const CString &dir_name)
{
    return Err::Nil();
}

} // namespace Path

} // namespace Platform
} // namespace Common
} // namespace AGS
