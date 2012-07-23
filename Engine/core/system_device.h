
//=============================================================================
//
// System Device interface
//
// [IKM] 2012-07-19: draft version
//
//=============================================================================
#ifndef __AGS_EE_CORE__SYSTEMDEVICE_H
#define __AGS_EE_CORE__SYSTEMDEVICE_H

#include "Common/core/err.h"
#include "Common/util/string.h"

namespace AGS
{
namespace Engine
{
namespace Core
{

// Using-declarations
using AGS::Common::Core::HErr;
using AGS::Common::Util::CString;

enum SystemDeviceType
{
    kSystemDevice_Audio,
    kSystemDevice_Controller,
    kSystemDevice_Data,
    kSystemDevice_Screen
};

class ISystemDevice
{
public:
    ISystemDevice(){}
    virtual ~ISystemDevice(){}

    virtual CString             GetName()   = 0;
    virtual SystemDeviceType    GetType()   = 0;

    virtual HErr                Install()   = 0;
    virtual HErr                Remove()    = 0;
    virtual HErr                Poll()      = 0;
};

} // namespace Core
} // namespace Engine
} // namespace AGS

#endif // __AGS_EE_CORE__SYSTEMDEVICE_H
