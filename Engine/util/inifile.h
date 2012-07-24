
//=============================================================================
//
// [IKM] 2012-07-09: ini file reader/writer draft
//
//=============================================================================
#ifndef __AGS_EE_UTILS__INIFILE_H
#define __AGS_EE_UTILS__INIFILE_H

#include "Common/core/err.h"
#include "Common/util/keyvaluetree.h"

namespace AGS
{

// Forward declarations
namespace Common
{
namespace Util
{
class CStream;
} // namespace Util
} // namespace Common

namespace Engine
{
namespace Util
{

// Using-declarations
using AGS::Common::Core::HErr;
using AGS::Common::Util::CString;
using AGS::Common::Util::CStream;
using AGS::Common::Util::CKeyValueTree;

class CINIFile : public CKeyValueTree
{
public:

    CINIFile();
    virtual ~CINIFile();

    // [IKM] some stuff from original AGS
    // Replace the filename part of complete path WASGV with INIFIL
    static void getdirec(const CString &inifil, CString *wasgv);

    virtual HErr    ReadAsTree   (CStream *in);
    virtual HErr    WriteAsTree  (CStream *out) const;
};


} // namespace Util
} // namespace Engine
} // namespace AGS

#endif // __AGS_EE_UTILS__INIFILE_H
