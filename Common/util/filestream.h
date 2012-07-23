
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

class AGS::Common::Util::CFile;

class CFileStream : public CStream
{
    CFile *f;

public:
    virtual ~CFileStream(){}

    // Is end of stream
    virtual bool    EOS() const { return false; }
    // Total length of stream (if known)
    virtual int32_t Length() const { return false; }
    // Current position (if known)
    virtual int32_t Position() const { return false; }
    virtual bool    CanRead() const { return false; }
    virtual bool    CanWrite() const { return false; }
    virtual bool    CanSeek() const { return false; }

    virtual int32_t Seek(StreamSeek seek, int32_t pos) { return 0; }

    virtual int8_t  ReadInt8() { return 0; }
    virtual int16_t ReadInt16() { return 0; }
    virtual int32_t ReadInt32() { return 0; }
    virtual int64_t ReadInt64() { return 0; }
    virtual int32_t Read(void *buffer, int32_t size) { return 0; }

    virtual void    WriteInt8(int8_t &val)   {}
    virtual void    WriteInt16(int16_t &val) {}
    virtual void    WriteInt32(int32_t &val) {}
    virtual void    WriteInt64(int64_t &val) {}
    virtual int32_t Write(const void *buffer, int32_t size) { return 0; }

    static CFileStream *Open(const CString &file_name, const CString &mode) { return new CFileStream(); }
    static CFileStream *CreateClient(CFile *f) { return new CFileStream(); }
    static CFileStream *CreateOwner(CFile *f) { return new CFileStream(); }

private:
    CFileStream() {}
};

} // namespace Util
} // namespace Common
} // namespace AGS

#endif // __AGS_CN_UTILS__FILESTREAM_H
