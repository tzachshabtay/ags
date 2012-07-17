
//=============================================================================
//
// Platform-independent FILE interface
//
// TODO: abstract interface wrapper around file handle
//
//=============================================================================
#ifndef __AGS_CN_UTIL__FILE_H
#define __AGS_CN_UTIL__FILE_H

#include <stdio.h>
// TODO: other platforms?

#if defined(LINUX_VERSION) || defined(MAC_VERSION)
long int filelength(int fhandle);
#endif

#if defined ALLEGRO_BIG_ENDIAN
#include "platform/bigend/bigend.h"
#else !ALLEGRO_BIG_ENDIAN
// Two functions to match those defined by bigend version by McCrea
short int   getshort(FILE *);
void        putshort(short int, FILE *);
#endif // !ALLEGRO_BIG_ENDIAN


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
namespace Util
{

//using AGS::Common::Util::CString;

struct CFile
{
    FILE *f;

    static CFile *Open(const CString &file_name, const CString &opt);
};

} // namespace Util
} // namespace Common
} // namespace AGS

#endif // __AGS_CN_UTIL__FILE_H
