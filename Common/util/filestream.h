
//=============================================================================
//
// [IKM] 2012-07-09: placeholder for the filestream class
//
//=============================================================================
#ifndef __AGS_CN_UTILS__FILESTREAM_H
#define __AGS_CN_UTILS__FILESTREAM_H

#include "Common/util/stream.h"
#include "Common/util/file.h"

namespace AGS
{
namespace Common
{
namespace Util
{

struct AGS::Common::Util::CFile;

struct CFileStream : public CStream
{
    CFile *f;

    static CFileStream *Open(const CString &file_name, const CString &mode) { return new CFileStream(); }
};

} // namespace Util
} // namespace Common
} // namespace AGS

#endif // __AGS_CN_UTILS__FILESTREAM_H
