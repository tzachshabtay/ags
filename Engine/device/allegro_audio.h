
//=============================================================================
//
// Allegro audio: a wrapper around allegro sound functions
//
// [IKM] 2012-07-24: draft version
//
//=============================================================================
#ifndef __AGS_EE_DEVICE__ALLEGROAUDIO_H
#define __AGS_EE_DEVICE__ALLEGROAUDIO_H

#include "Engine/device/device_audio.h"

namespace AGS
{
namespace Engine
{
namespace Device
{

// Using-declarations
using AGS::Common::Util::CString;

class CAllegroAudio : public CAudioDevice
{
public:
    CAllegroAudio();
    virtual ~CAllegroAudio();

    virtual CString             GetName();

    virtual HErr                Install();
    virtual HErr                Remove();
    virtual HErr                Poll();

private:
    bool _useExtraSoundOffset;
};

} // namespace Device
} // namespace Engine
} // namespace AGS

#endif // __AGS_EE_DEVICE__ALLEGROAUDIO_H
