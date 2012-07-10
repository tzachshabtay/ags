
#include <string.h>
#include "Engine/util/inifile.h"

namespace AGS
{
namespace Engine
{
namespace Util
{

using AGS::Common::Core::CErrorHandle;

CINIFile::CINIFile()
{
}

CINIFile::~CINIFile()
{
}

HErr CINIFile::ReadAsTree   (CStream *in)
{
    return CErrorHandle::Nil();
}

HErr CINIFile::WriteAsTree  (CStream *out) const
{
    return CErrorHandle::Nil();
}

CINIFile *CINIFile::Open(const CString &file_name, const CString &mode)
{
    return new CINIFile(); // 0_o
}

// [IKM] some stuff from original AGS
// Replace the filename part of complete path WASGV with INIFIL
void CINIFile::getdirec(const CString &inifil, CString *wasgv)
{
    int u = strlen(wasgv->s) - 1;

    for (u = strlen(wasgv->s) - 1; u >= 0; u--) {
        if ((wasgv->s[u] == '\\') || (wasgv->s[u] == '/')) {
            memcpy(&wasgv->s[u + 1], inifil.s, strlen(inifil.s) + 1);
            break;
        }
    }

    if (u <= 0) {
        // no slashes - either the path is just "f:acwin.exe"
        if (strchr(wasgv->s, ':') != NULL)
            memcpy(strchr(wasgv->s, ':') + 1, inifil.s, strlen(inifil.s) + 1);
        // or it's just "acwin.exe" (unlikely)
        else
            strcpy(wasgv->s, inifil.s);
    }
}

} // namespace Util
} // namespace Engine
} // namespace AGS
