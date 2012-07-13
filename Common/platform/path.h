
//=============================================================================
//
// System path helpers
//
//=============================================================================
#ifndef __AGS_CN_PLATFORM__PATH_H
#define __AGS_CN_PLATFORM__PATH_H

#include "Common/core/err.h"
#include "Common/util/string.h"

namespace AGS
{
namespace Common
{
namespace Platform
{

using AGS::Common::Core::HErr;
using AGS::Common::Util::CString;

namespace Path
{

// Returns true if the string is recognized as a valid path (starts with X:/ or \\)
bool    IsPath(const CString &file_name);

CString FixFileName(const CString &file_name);
// Gets only file name (with extension)
CString GetFileName(const CString &file_name);
// Cuts everything beyond the last '\\' (including separator char)
CString GetParentPath(const CString &file_name);
// Gets old 8.3-format path alias
CString GetShortPath(const CString &path);
// Concatenates two paths, making them one (e.g. C:/Games + MyAGSGame/demo.exe = C:/Games/MyAGSGame/demo.exe)
CString MakePath(const CString &parent_path, const CString &child_path);

// Gets current working directory
CString GetCurrentDirectory();
HErr    SetCurrentDirectory(const CString &dir_name);

} // namespace Path

} // namespace Platform
} // namespace Common
} // namespace AGS

#endif // __AGS_CN_PLATFORM__PATH_H
