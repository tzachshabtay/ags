
//=============================================================================
//
// System path helpers
//
//=============================================================================
#ifndef __AGS_CN_PLATFORM__PATH_H
#define __AGS_CN_PLATFORM__PATH_H

#include "Common/util/string.h"

namespace AGS
{
namespace Common
{
namespace Platform
{

using AGS::Common::Util::CString;

namespace Path
{

CString FixFileName(const CString &file_name);
// Gets current working directory
CString GetCurrentDirectory();

} // namespace Path

} // namespace Platform
} // namespace Common
} // namespace AGS

#endif // __AGS_CN_PLATFORM__PATH_H
