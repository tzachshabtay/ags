
//=============================================================================
//
// Mouse definitions
//
// TODO: Think of moving this somewhere else (not Device, probably not even Engine)
//
//=============================================================================
#ifndef __AGS_EE_DEVICE__MOUSEDEFINES_H
#define __AGS_EE_DEVICE__MOUSEDEFINES_H

#include "Common/core/types.h"

namespace AGS
{
namespace Engine
{
namespace Device
{

enum
{
    kMB_None    = 0,
    kMB_Left    = 0x01,
    kMB_Right   = 0x02,
    kMB_Middle  = 0x04
};

typedef int8_t MouseButtons;

} // namespace Device
} // namespace Engine
} // namespace AGS

#endif // __AGS_EE_DEVICE__MOUSEDEFINES_H
