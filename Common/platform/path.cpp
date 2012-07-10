
#include "Common/platform/path.h"
#include "allegro/file.h"

namespace AGS
{
namespace Common
{
namespace Platform
{

namespace Path
{

CString FixFileName(const CString &file_name)
{
    char *sz_buf = file_name.s;

    fix_filename_case(sz_buf);
    fix_filename_slashes(sz_buf);

    return CString(sz_buf); // duh, won't work, need normal code
}

CString GetCurrentDirectory()
{
    char sz_buf[256];
    _getcwd (sz_buf, 255);

    return CString(sz_buf); // duh, won't work, need normal code
}

} // namespace Path

} // namespace Platform
} // namespace Common
} // namespace AGS
