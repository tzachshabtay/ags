
//=============================================================================
//
// [IKM] 2012-07-09: draft of the key-value tree interface, a base class for
// reading/writing named values; could be a parent for INI- and XML-like
// structure managers.
//
//=============================================================================
#ifndef __AGS_CN_UTILS__KEYVALUETREE_H
#define __AGS_CN_UTILS__KEYVALUETREE_H

#include "Common/core/err.h"
#include "Common/util/string.h"

namespace AGS
{
namespace Common
{
namespace Util
{

// Forward declarations
class CStream;
// Using-declarations
using AGS::Common::Core::HErr;

class CKeyValueTree
{
public:
    virtual ~CKeyValueTree() {}

    // Reads/writes full tree contents as a default tree format
    // These functions are pure-virtual since actual implementation
    // depends on format type (INI, XML or else)
    virtual HErr    ReadAsTree   (CStream *in)  = 0;
    virtual HErr    WriteAsTree  (CStream *out) const = 0;

    // Reads/writes full tree contents as a packed binary form
    virtual HErr    ReadAsBinary (CStream *in);
    virtual HErr    WriteAsBinary(CStream *out) const;

    // Creates sub-tree if it doesn't exist, otherwise returns existing sub-tree
    virtual CKeyValueTree *CreateTree(const CString &name);
    // Seeks for existing sub-tree, returns NULL if it does not exists
    virtual CKeyValueTree *FindTree  (const CString &name) const;

    // Reads value only if exists
    virtual bool    TryReadInt8  (const CString &name, int8_t  *value) const;
    virtual bool    TryReadInt16 (const CString &name, int16_t *value) const;
    virtual bool    TryReadInt32 (const CString &name, int32_t *value) const;
    virtual bool    TryReadString(const CString &name, const CString *value) const;

    // Reads value and returns default if it does not exists
    virtual int8_t  ReadInt8  (const CString &name, int8_t  def_value = 0) const;
    virtual int16_t ReadInt16 (const CString &name, int16_t def_value = 0) const;
    virtual int32_t ReadInt32 (const CString &name, int32_t def_value = 0) const;
    virtual CString ReadString(const CString &name, const CString &def_value = "") const;

    // Writes value (implicitly creates node if it does not exist)
    virtual void    WriteInt8  (const CString &name, int8_t  value);
    virtual void    WriteInt16 (const CString &name, int16_t value);
    virtual void    WriteInt32 (const CString &name, int32_t value);
    virtual void    WriteString(const CString &name, const CString &value);
};

} // namespace Util
} // namespace Common
} // namespace AGS

#endif // __AGS_CN_UTILS__KEYVALUETREE_H
