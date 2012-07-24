
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

// CErrorHandle is a simple wrapper over reference-counted error
// object, which is stored and managed internally by error-handling
// system.
// When reference count reaches zero, the error system is notified;
// it then displays error and/or writes it to the application log.
// This logic makes sure that an error created somewhere deep in the
// nested functions won't get lost unnoticed.
//
// NOTE: currently a placeholder with no actual data
class CErrorHandle
{
public:
    // Safe check
    inline bool IsNil() const { return err_obj == NULL; }
    inline int GetErrorCodeOrSomething() const { return -1; }

    CErrorHandle()
    {
        err_obj = NULL;
    }

    CErrorHandle(const CErrorHandle &err)
    {
        err_obj = err.err_obj;
    }
private:
    void *err_obj;
};

namespace Err
{
// "No error" object
inline CErrorHandle Nil()                               { return CErrorHandle(); }
inline CErrorHandle FromCode   (int32_t err_code)       { return CErrorHandle(); }
inline CErrorHandle FromString (const CString &err_msg) { return CErrorHandle(); }
};

typedef CErrorHandle HErr;

} // namespace Core
} // namespace Common
} // namespace AGS

#endif // __AGS_CN_CORE__ERR_H
