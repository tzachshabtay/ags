
#include "Common/util/keyvaluetree.h"

namespace AGS
{
namespace Common
{
namespace Util
{

// Using-declarations
using AGS::Common::Core::CErrorHandle;

HErr CKeyValueTree::ReadAsBinary (CStream *in)
{
    return CErrorHandle::Nil();
}

HErr CKeyValueTree::WriteAsBinary(CStream *out) const
{
    return CErrorHandle::Nil();
}

CKeyValueTree *CKeyValueTree::CreateTree(const CString &name)
{
    return NULL;
}

CKeyValueTree *CKeyValueTree::FindTree(const CString &name) const
{
    return NULL;
}

bool CKeyValueTree::TryReadInt8(const CString &name, int8 *value) const
{
    return false;
}

bool CKeyValueTree::TryReadInt16(const CString &name, int16 *value) const
{
    return false;
}

bool CKeyValueTree::TryReadInt32(const CString &name, int32 *value) const
{
    return false;
}

bool CKeyValueTree::TryReadString(const CString &name, const CString *value) const
{
    return false;
}

int8 CKeyValueTree::ReadInt8(const CString &name, int8 def_value) const
{
    return 0;
}

int16 CKeyValueTree::ReadInt16(const CString &name, int16 def_value) const
{
    return 0;
}

int32 CKeyValueTree::ReadInt32(const CString &name, int32 def_value) const
{
    return 0;
}

CString CKeyValueTree::ReadString(const CString &name, const CString &def_value) const
{
    return "";
}

void CKeyValueTree::WriteInt8(const CString &name, int8 value)
{
}

void CKeyValueTree::WriteInt16(const CString &name, int16 value)
{
}

void CKeyValueTree::WriteInt32 (const CString &name, int32 value)
{
}

void CKeyValueTree::WriteString(const CString &name, const CString &value)
{
}

} // namespace Util
} // namespace Common
} // namespace AGS
