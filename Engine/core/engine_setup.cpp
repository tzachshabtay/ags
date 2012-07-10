
#include "Common/core/err.h"
#include "Engine/core/engine_setup.h"
#include "wgt2allg.h"
#include "sprcache.h"
#include "ac/gamestate.h"

extern SpriteCache spriteset;
extern GameState play;

namespace AGS
{
namespace Engine
{
namespace Core
{

using AGS::Common::Core::CErrorHandle;
using AGS::Common::Util::CKeyValueTree;

CEngineSetup::CEngineSetup()
{
    SetDefaults();
}

void CEngineSetup::SetDefaults()
{
}

HErr CEngineSetup::Read (CStream *in)
{
    return CErrorHandle::Nil();
}

HErr CEngineSetup::Write(CStream *out)
{
    return CErrorHandle::Nil();
}

HErr CEngineSetup::ReadCFG (CKeyValueTree *tree)
{
    CKeyValueTree *sound_tree = tree->CreateTree("sound");
#ifndef WINDOWS_VERSION
    Digicard=sound_tree->ReadInt32("digiid");
    Midicard=sound_tree->ReadInt32("midiid");
#else
    
        int idx = sound_tree->ReadInt32("digiwinindx", 0);
        if (idx == 0)
            idx = DIGI_DIRECTAMX(0);
        else if (idx == 1)
            idx = DIGI_WAVOUTID(0);
        else if (idx == 2)
            idx = DIGI_NONE;
        else if (idx == 3) 
            idx = DIGI_DIRECTX(0);
        else 
            idx = DIGI_AUTODETECT;
        Digicard = idx;

        idx = sound_tree->ReadInt32("midiwinindx", 0);
        if (idx == 1)
            idx = MIDI_NONE;
        else if (idx == 2)
            idx = MIDI_WIN32MAPPER;
        else
            idx = MIDI_AUTODETECT;
        Midicard = idx;

        if (Digicard < 0)
            Digicard = DIGI_AUTODETECT;
        if (Midicard < 0)
            Midicard = MIDI_AUTODETECT;
#endif

        CKeyValueTree *misc_tree = tree->CreateTree("misc");

#if defined(WINDOWS_VERSION) || defined(LINUX_VERSION) || defined(MAC_VERSION)
        Windowed = misc_tree->ReadInt32("windowed");
        if (Windowed < 0)
            Windowed = 0;
#endif

        Refresh = misc_tree->ReadInt32("refresh", 0);
        EnableAntialiasing = misc_tree->ReadInt32("antialias", 0);
        ForceHicolorMode = misc_tree->ReadInt32("notruecolor", 0);
        EnableSideBorders = misc_tree->ReadInt32("sideborders", 0);

#if defined(IOS_VERSION) || defined(PSP_VERSION) || defined(ANDROID_VERSION)
        // PSP: Letterboxing is not useful on the PSP.
        ForceLetterbox = 0;
#else
        ForceLetterbox = misc_tree->ReadInt32("forceletterbox", 0);
#endif

        if (EnableAntialiasing < 0)
            EnableAntialiasing = 0;
        if (ForceHicolorMode < 0)
            ForceHicolorMode = 0;
        if (EnableSideBorders < 0)
            EnableSideBorders = 1;

        // This option is backwards (usevox is 0 if no_speech_pack)
        NoSpeechPack = sound_tree->ReadInt8("usespeech", 0);
        if (NoSpeechPack == 0)
            NoSpeechPack = 1;
        else
            NoSpeechPack = 0;

        DataFilesDir = misc_tree->ReadString("datadir");
        if (DataFilesDir.IsEmpty())
            DataFilesDir = ".";
        // strip any trailing slash
#if defined(LINUX_VERSION) || defined(MAC_VERSION)
        if (DataFilesDir[strlen(data_files_dir)-1] == '/')
            DataFilesDir[strlen(data_files_dir)-1] = 0;
#else
        if ((DataFilesDir.GetLength() < 4) && (DataFilesDir[1] == ':'))
        { }  // if the path is just  d:\  don't strip the slash
        else if (DataFilesDir[DataFilesDir.GetLength()-1] == '\\')
            DataFilesDir[DataFilesDir.GetLength()-1] = 0;
#endif

        MainDataFilename = misc_tree->ReadString("datafile");
        if (MainDataFilename.IsEmpty())
            MainDataFilename = "ac2game.dat";

#if defined(IOS_VERSION) || defined(PSP_VERSION) || defined(ANDROID_VERSION)
        // PSP: No graphic filters are available.
        GfxFilterID = NULL;
#else
        GfxFilterID = misc_tree->ReadString("gfxfilter");
#endif

#if defined(LINUX_VERSION) || defined(MAC_VERSION)
        GfxDriverID = "DX5";
#else
        GfxDriverID = misc_tree->ReadString("gfxdriver");
#endif

        CKeyValueTree *lang_tree = tree->CreateTree("language");
        Translation = lang_tree->ReadString("translation");

#if !defined(IOS_VERSION) && !defined(PSP_VERSION) && !defined(ANDROID_VERSION)
        // PSP: Don't let the setup determine the cache size as it is always too big.
        int tempint = misc_tree->ReadInt32("cachemax");
        if (tempint > 0)
            spriteset.maxCacheSize = tempint * 1024;
#endif

        CString repfile = misc_tree->ReadString("replay");
        if (!repfile.IsEmpty()) {
            ReplayFile = repfile;
            play.playback = 1;
        }
        else
            play.playback = 0;

    return CErrorHandle::Nil();
}

HErr CEngineSetup::WriteCFG(CKeyValueTree *tree)
{
    return CErrorHandle::Nil();
}

} // namespace Core
} // namespace Engine
} // namespace AGS