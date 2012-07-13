
#include "Common/util/keyvaluetree.h"

namespace AGS
{
namespace Common
{
namespace Util
{

// Using-declarations
using AGS::Common::Core::CErrorHandle;
namespace Err = AGS::Common::Core::Err;

HErr CKeyValueTree::ReadAsBinary (CStream *in)
{
    return Err::Nil();
}

HErr CKeyValueTree::WriteAsBinary(CStream *out) const
{
    return Err::Nil();
}

CKeyValueTree *CKeyValueTree::CreateTree(const CString &name)
{
    return NULL;
}

CKeyValueTree *CKeyValueTree::FindTree(const CString &name) const
{
    return NULL;
}

bool CKeyValueTree::TryReadInt8(const CString &name, int8_t *value) const
{
    return false;
}

bool CKeyValueTree::TryReadInt16(const CString &name, int16_t *value) const
{
    return false;
}

bool CKeyValueTree::TryReadInt32(const CString &name, int32_t *value) const
{
    return false;
}

bool CKeyValueTree::TryReadString(const CString &name, const CString *value) const
{
    return false;
}

int8_t CKeyValueTree::ReadInt8(const CString &name, int8_t def_value) const
{
    return 0;
}

int16_t CKeyValueTree::ReadInt16(const CString &name, int16_t def_value) const
{
    return 0;
}

int32_t CKeyValueTree::ReadInt32(const CString &name, int32_t def_value) const
{
    return 0;
}

CString CKeyValueTree::ReadString(const CString &name, const CString &def_value) const
{
    return "";
}

void CKeyValueTree::WriteInt8(const CString &name, int8_t value)
{
}

void CKeyValueTree::WriteInt16(const CString &name, int16_t value)
{
}

void CKeyValueTree::WriteInt32 (const CString &name, int32_t value)
{
}

void CKeyValueTree::WriteString(const CString &name, const CString &value)
{
}

} // namespace Util
} // namespace Common
} // namespace AGS
