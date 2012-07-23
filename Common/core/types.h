
//=============================================================================
//
// AGS types
//
//=============================================================================
#ifndef __AGS_CN_CORE__TYPES_H
#define __AGS_CN_CORE__TYPES_H

#if !defined (WINDOWS_VERSION)
#include <stdint.h>
#else // WINDOWS_VERSION

#undef NULL
#define NULL 0

namespace AGS
{
    // These ifndefs are temporary, to evade conflicts with allegro headers;
#ifndef int8_t
    typedef char            int8_t;
#endif
#ifndef int16_t
    typedef short int       int16_t;
#endif
#ifndef int32_t
    typedef long int        int32_t;
#endif
#ifndef int64_t
    typedef long long int   int64_t;
#endif

    typedef unsigned char   byte;

#ifndef intptr_t
// [IKM] something like that... (based on JJS's explanation)
#if defined (X64_BIT)
    typedef long long int   intptr_t;
#else
    typedef long int        intptr_t;
#endif
#endif

} // namespace AGS
#endif // WINDOWS_VERSION

#endif // __AGS_CN_CORE__TYPES_H
