
//=============================================================================
//
// [IKM] 2012-07-09: placeholder for the memorystream class
//
//=============================================================================
#ifndef __AGS_CN_UTILS__MEMORYSTREAM_H
#define __AGS_CN_UTILS__MEMORYSTREAM_H

#include "Common/util/stream.h"

namespace AGS
{
namespace Common
{
namespace Util
{

struct CMemoryStream : public CStream
{
    void *bytes;
    int length;
};

} // namespace Util
} // namespace Common
} // namespace AGS

#endif // __AGS_CN_UTILS__MEMORYSTREAM_H
