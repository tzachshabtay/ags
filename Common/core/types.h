
//=============================================================================
//
// AGS types
//
//=============================================================================
#ifndef __AGS_CN_CORE__TYPES_H
#define __AGS_CN_CORE__TYPES_H

#undef NULL
#define NULL 0

namespace AGS
{
    typedef char            int8;
    typedef short int       int16;
#ifndef int32
    typedef long int        int32;
#endif
    typedef long long int   int64;

#ifndef intptr_t
    // [IKM] something like that... (based on JJS's explanation)
#if defined (WINDOWS_VERSION)
    #if defined (X64_BIT)
        typedef long long int   intptr_t;
    #else
        typedef long int        intptr_t;
    #endif
#else
    typedef long int            intptr_t;
#endif
#endif

} // namespace AGS

#endif // __AGS_CN_CORE__TYPES_H
