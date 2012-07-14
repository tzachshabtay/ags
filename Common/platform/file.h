
//=============================================================================
//
// Platform-independent FILE interface
//
// TODO: abstract interface wrapper around file handle
//
//=============================================================================
#ifndef __AGS_CN_PLATFORM__FILE_H
#define __AGS_CN_PLATFORM__FILE_H

#include <stdio.h>
// TODO: other platforms?

#if defined(LINUX_VERSION) || defined(MAC_VERSION)
long int filelength(int fhandle);
#endif

// Two functions to match those defined by bigend version by McCrea
#if !defined ALLEGRO_BIG_ENDIAN
short int   getshort(FILE *);
void        putshort(short int, FILE *);
#endif // !ALLEGRO_BIG_ENDIAN

#include "platform/bigend.h"

//=============================================================================
//
// [IKM] 2012-07-09: placeholder for the file class
//
//=============================================================================

#include "Common/util/string.h"

namespace AGS
{
namespace Common
{
namespace Platform
{

using AGS::Common::Util::CString;

struct CFile
{
    FILE *f;

    static CFile *Open(const CString &file_name, const CString &opt);
};

} // namespace Platform
} // namespace Common
} // namespace AGS

#endif // __AGS_CN_PLATFORM__FILE_H
