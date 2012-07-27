
//=============================================================================
//
// [IKM] 2012-07-09: placeholder for the stream class
//
//=============================================================================
#ifndef __AGS_CN_UTIL__STREAM_H
#define __AGS_CN_UTIL__STREAM_H

#include "Common/core/types.h"

namespace AGS
{
namespace Common
{
namespace Util
{

enum StreamSeek
{
    kSeekBegin,
    kSeekEnd,
    kSeekCurrent
};

class CStream
{
public:
    virtual ~CStream(){}

    // Is end of stream
    virtual bool    EOS() const = 0;
    // Total length of stream (if known)
    virtual int32_t Length() const = 0;
    // Current position (if known)
    virtual int32_t Position() const = 0;
    virtual bool    CanRead() const = 0;
    virtual bool    CanWrite() const = 0;
    virtual bool    CanSeek() const = 0;

    virtual int32_t Seek(StreamSeek seek, int32_t pos) = 0;

    virtual int8_t  ReadInt8()  = 0;
    virtual int16_t ReadInt16() = 0;
    virtual int32_t ReadInt32() = 0;
    virtual int64_t ReadInt64() = 0;
    virtual int32_t Read(void *buffer, int32_t size) = 0;

    virtual void    WriteInt8(int8_t &val)   = 0;
    virtual void    WriteInt16(int16_t &val) = 0;
    virtual void    WriteInt32(int32_t &val) = 0;
    virtual void    WriteInt64(int64_t &val) = 0;
    virtual int32_t Write(const void *buffer, int32_t size) = 0;
};

} // namespace Util
} // namespace Common
} // namespace AGS

#endif // __AGS_CN_UTIL__STREAM_H
