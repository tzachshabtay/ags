
//=============================================================================
//
// [IKM] 2012-07-09: placeholder for AGS error system
//
//=============================================================================
#ifndef __AGS_CN_CORE__ERR_H
#define __AGS_CN_CORE__ERR_H

#include "Common/core/types.h"

namespace AGS
{
namespace Common
{
namespace Core
{

struct CErrorHandle
{
    void *err;

    // "No error" object
    inline static CErrorHandle *Nil() { return NULL; }
};

typedef CErrorHandle *HErr;

} // namespace Core
} // namespace Common
} // namespace AGS

#endif // __AGS_CN_CORE__ERR_H
