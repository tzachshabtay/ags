
//=============================================================================
//
// [IKM] 2012-07-09: placeholder for AGS error system
//
//=============================================================================
#ifndef __AGS_CN_CORE__ERR_H
#define __AGS_CN_CORE__ERR_H

#include "Common/core/types.h"
#include "Common/util/string.h"

namespace AGS
{
namespace Common
{
namespace Core
{

// Using-declarations
using AGS::Common::Util::CString;

struct CErrorHandle
{
    void *err;

    // Safe check
    inline bool IsNil() { return this == NULL; }
};

namespace Err
{
// "No error" object
inline CErrorHandle *Nil()                               { return NULL; }
inline CErrorHandle *FromCode   (int32_t err_code)       { return NULL; }
inline CErrorHandle *FromString (const CString &err_msg) { return NULL; }
};

typedef CErrorHandle *HErr;

} // namespace Core
} // namespace Common
} // namespace AGS

#endif // __AGS_CN_CORE__ERR_H
