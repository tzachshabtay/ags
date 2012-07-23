
#include "Common/core/out.h"
#include "Engine/core/agsengine.h"
#include "Engine/core/engine_setup.h"
#include "Engine/device/allegro_audio.h"
#include "Engine/platform/base/agsplatformdriver.h"

extern int psp_audio_enabled;
extern int psp_midi_enabled;

namespace AGS
{
namespace Engine
{
namespace Device
{

// Using-declarations
using AGS::Engine::Core::CAGSEngine;
using AGS::Engine::Core::CEngineSetup;
namespace Err = AGS::Common::Core::Err;
namespace Out = AGS::Common::Core::Out;

CAllegroAudio::CAllegroAudio()
{
}

CAllegroAudio::~CAllegroAudio()
{
}

CString CAllegroAudio::GetName()
{
    return "alAudio";
}

HErr CAllegroAudio::Install()
{
    AGSPlatformDriver *platform = AGSPlatformDriver::GetDriver();
    CEngineSetup &setup = CAGSEngine::GetSetup();
#ifdef WINDOWS_VERSION
    // don't let it use the hardware mixer verion, crashes some systems
    //if ((setup.Digicard == DIGI_AUTODETECT) || (setup.Digicard == DIGI_DIRECTX(0)))
    //    setup.Digicard = DIGI_DIRECTAMX(0);

    if (setup.Digicard == DIGI_DIRECTX(0)) {
        // DirectX mixer seems to buffer an extra sample itself
        _useExtraSoundOffset = true;
    }

    // if the user clicked away to another app while we were
    // loading, DirectSound will fail to initialize. There doesn't
    // seem to be a solution to force us back to the foreground,
    // because we have no actual visible window at this time

#endif

    Out::Notify("Initialize sound drivers");

    // PSP: Disable sound by config file.
    if (!psp_audio_enabled)
    {
        setup.Digicard = DIGI_NONE;
        setup.Midicard = MIDI_NONE;
    }

    if (!psp_midi_enabled)
        setup.Midicard = MIDI_NONE;

    if (install_sound(setup.Digicard,setup.Midicard,NULL)!=0) {
        reserve_voices(-1,-1);
        setup.ModPlayer=0;
        setup.MP3Player=0;
        if (install_sound(setup.Digicard,setup.Midicard,NULL)!=0) {
            if ((setup.Digicard != DIGI_NONE) && (setup.Midicard != MIDI_NONE)) {
                // only flag an error if they wanted a sound card
                platform->DisplayAlert("\nUnable to initialize your audio hardware.\n"
                    "[Problem: %s]\n",allegro_error);
            }
            reserve_voices(0,0);
            install_sound(DIGI_NONE, MIDI_NONE, NULL);
            setup.Digicard = DIGI_NONE;
            setup.Midicard = MIDI_NONE;
        }
    }

    CAGSEngine::SetEIP(-181);
    return Err::Nil();
}

HErr CAllegroAudio::Remove()
{
    return Err::Nil();
}

HErr CAllegroAudio::Poll()
{
    return Err::Nil();
}

} // namespace Device
} // namespace Engine
} // namespace AGS
