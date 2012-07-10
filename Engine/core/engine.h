
//=============================================================================
//
// AGS Engine class
//
// [IKM] 2012-07-09: draft version
//
//=============================================================================
#ifndef __AGS_EE_CORE__ENGINE_H
#define __AGS_EE_CORE__ENGINE_H

#include "Common/core/err.h"
#include "Engine/platform/platform_api.h"
#include "Engine/util/cmdargs.h"

namespace AGS
{
namespace Engine
{

// Forward declarations
namespace Util
{
class CCmdArgs;
}

namespace Core
{

// Forward declarations
struct CEngineSetup;

// Using-declarations
using AGS::Common::Core::HErr;
using AGS::Common::Util::CString;
using AGS::Engine::Util::CCmdArgs;

class CAGSEngine
{
    friend void atexit_handler();
public:
    // Explicitly constructs the instance of AGS Engine; issues a warning if called more than once
    static HErr         ConstructInstance();
    // Gets the instance of AGS Engine; does not implicitly constructs one
    static CAGSEngine   *GetInstance();

    CString         GetEngineVersion() const;
    void            SetEIP(int32 eip);
    int32           GetEIP() const;
    CEngineSetup   *GetSetup() const;

    //-------------------------------------------------------------------------
    // A copy of original engine init functions
    //
    
    HErr        Initialize(const CCmdArgs &cmdargs);
    HErr        InitializeWithExceptionHandling(const CCmdArgs &cmdargs);
    //-------------------------------------------------------------------------

protected:

    HErr ReadConfigFile(const CString &file_name);

    //-------------------------------------------------------------------------
    // A copy of original engine init functions
    //
    int engine_init_allegro();
    void winclosehook();
    void engine_setup_window();
    int engine_check_run_setup(int argc,char*argv[]);
    void engine_force_window();
    int engine_init_game_data_external(int argc,char*argv[]);
    int engine_init_game_data_internal(int argc,char*argv[]);
    void initialise_game_file_name();
    int engine_init_game_data(int argc,char*argv[]);
    void engine_init_fonts();
    int engine_init_mouse();
    int engine_check_memory();
    void engine_init_rooms();
    int engine_init_speech();
    int engine_init_music();
    void engine_init_keyboard();
    void engine_init_timer();
    void engine_init_sound();
    void engine_init_debug();
    void atexit_handler();
    void engine_init_exit_handler();
    void engine_init_rand();
    void engine_init_pathfinder();
    void engine_pre_init_gfx();
    int engine_load_game_data();
    int engine_check_register_game();
    void engine_init_title();
    void engine_init_directories();
    int check_write_access();
    int engine_check_disk_space();
    // [IKM] I have a feeling this should be merged with engine_init_fonts
    int engine_check_fonts();
    void engine_init_modxm_player();
    void show_preload ();
    void engine_show_preload();
    int engine_init_sprites();
    void engine_setup_screen();
    void init_game_settings();
    void engine_init_game_settings();
    void engine_init_game_shit();
#if defined(PSP_VERSION)
    // PSP: Workaround for sound stuttering. Do sound updates in its own thread.
    int update_mp3_thread(SceSize args, void *argp);;
#elif (defined(LINUX_VERSION) && !defined(PSP_VERSION)) || defined(MAC_VERSION)
    void* update_mp3_thread(void* arg);
#elif defined(WINDOWS_VERSION)
    DWORD WINAPI update_mp3_thread(LPVOID lpParam);
#endif

    void engine_start_multithreaded_audio();
    void engine_prepare_to_start_game();
    //-------------------------------------------------------------------------

private:
    CAGSEngine();

    // Platform-specific override
    virtual void        OverrideSetup();

    // The only instance of the AGS Engine
    static CAGSEngine   *_theEngine;
    // EIP = Extended Instruction Pointer; here it is simply an integer, which
    // stores numerical id of last "checkpoint" in program execution
    int32               _eip;
    CEngineSetup        *_theSetup;
    CString             _configFileDefName;
    CString             _configFileName;
    bool                _ignoreConfigFile;
};

} // namespace Core
} // namespace Engine
} // namespace AGS

#endif // __AGS_EE_CORE__ENGINE_H
