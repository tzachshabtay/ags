
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
// Temporarily inline
inline void Debug          (const CString &msg, ...){};
inline void Debug          (const CString &fnname, const CString &msg, ...){};
inline void Notify         (const CString &msg, ...){};
inline void Notify         (const CString &fnname, const CString &msg, ...){};
inline void Warn           (const CString &msg, ...){};
inline void Warn           (const CString &fnname, const CString &msg, ...){};
inline void HandledErr     (const CString &msg, ...){};
inline void HandledErr     (const CString &fnname, const CString &msg, ...){};
inline void UnhandledErr   (const CString &msg, ...){};
inline void UnhandledErr   (const CString &fnname, const CString &msg, ...){};
inline void FatalErr       (const CString &msg, ...){};
inline void FatalErr       (const CString &fnname, const CString &msg, ...){};
inline void FnIn           (const CString &fnname){};
inline void FnOut          (const CString &fnname){};

} // namespace Out

} // namespace Core
} // namespace Common
} // namespace AGS

#endif // __AGS_CN_CORE__OUT_H
