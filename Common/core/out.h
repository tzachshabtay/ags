
//=============================================================================
//
// [IKM] 2012-07-09: placeholder for AGS logging system
//
//=============================================================================
#ifndef __AGS_CN_CORE__OUT_H
#define __AGS_CN_CORE__OUT_H

#include "Common/util/string.h"

namespace AGS
{
namespace Common
{
namespace Core
{

using AGS::Common::Util::CString;

namespace Out
{

// Convenience functions, with regard to verbosity settings
void Debug          (const CString &msg, ...){};
void Debug          (const CString &fnname, const CString &msg, ...){};
void Notify         (const CString &msg, ...){};
void Notify         (const CString &fnname, const CString &msg, ...){};
void Warn           (const CString &msg, ...){};
void Warn           (const CString &fnname, const CString &msg, ...){};
void HandledErr     (const CString &msg, ...){};
void HandledErr     (const CString &fnname, const CString &msg, ...){};
void UnhandledErr   (const CString &msg, ...){};
void UnhandledErr   (const CString &fnname, const CString &msg, ...){};
void FatalErr       (const CString &msg, ...){};
void FatalErr       (const CString &fnname, const CString &msg, ...){};
void FnIn           (const CString &fnname){};
void FnOut          (const CString &fnname){};

} // namespace Out

} // namespace Core
} // namespace Common
} // namespace AGS

#endif // __AGS_CN_CORE__OUT_H
