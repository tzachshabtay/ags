
//=============================================================================
//
// [IKM] 2012-07-09: placeholder for the filestream class
//
//=============================================================================
#ifndef __AGS_CN_UTILS__FILESTREAM_H
#define __AGS_CN_UTILS__FILESTREAM_H

#include "Common/util/stream.h"
#include "Common/platform/file.h"

namespace AGS
{
namespace Common
{
namespace Util
{

struct AGS::Common::Platform::CFile;

struct CFileStream : public CStream
{
    CFile *f;
};

} // namespace Util
} // namespace Common
} // namespace AGS

#endif // __AGS_CN_UTILS__FILESTREAM_H
