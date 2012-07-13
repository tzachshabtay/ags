
//=============================================================================
//
// Platform API declarations and definition
//
//=============================================================================
#ifndef __AGS_EE_PLATFORM__PLATFORMAPI_H
#define __AGS_EE_PLATFORM__PLATFORMAPI_H

#include "wgt2allg.h" // FIXME! -- temp

#if defined (WINDOWS_VERSION)

// Undef conflicting names
#undef GetCurrentDirectory
#undef SetCurrentDirectory

#else

#endif

#endif // __AGS_EE_PLATFORM__PLATFORMAPI_H
