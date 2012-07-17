
//=============================================================================
//
// AGS Engine class
//
// [IKM] 2012-07-09: draft version
//
//=============================================================================
#ifndef __AGS_EE_CORE__AGSENGINE_H
#define __AGS_EE_CORE__AGSENGINE_H

#include "Common/core/err.h"
#include "Common/platform/base/platform_api.h"
#include "Engine/util/cmdargs.h"

#if defined(WINDOWS_VERSION) && !defined(_DEBUG)
#define USE_CUSTOM_EXCEPTION_HANDLER
#endif

// Forward declaration
struct AGSPlatformDriver;

namespace AGS
{
namespace Engine
{

// Forward declarations
namespace Game
{
class CAGSGame;
}
namespace Util
{
class CCmdArgs;
}

//-----------------------------------------------------------------------------

namespace Core
{

// Forward declarations
struct CEngineSetup;

// Using-declarations
using AGS::Common::Core::HErr;
using AGS::Common::Util::CString;
using AGS::Engine::Game::CAGSGame;
using AGS::Engine::Util::CCmdArgs;

//-----------------------------------------------------------------------------

class CAGSEngine
{
    friend void atexit_handler();
public:
    ~CAGSEngine();

    // Explicitly constructs the instance of AGS Engine; issues a warning if called more than once
    static HErr         CreateInstance();
    // Gets the instance of AGS Engine; does not implicitly constructs one
    static CAGSEngine   *GetInstance();

    CString         GetEngineVersion() const;
    void            SetEIP(int32_t eip);
    int32_t         GetEIP() const;

    CEngineSetup   *GetSetup() const;
    CAGSGame       *GetGame() const;

    HErr            Initialize(const CCmdArgs &cmdargs);
    HErr            StartUp();
    HErr            StartUpWithExceptionHandling();

protected:

    HErr ProcessCmdLine(const CCmdArgs &cmdargs);
    HErr ReadConfigFile();
    HErr InitAllegro();
    HErr InitWindow();
    bool SetGameDataFileName();
    HErr InitGameDataFile();
    int  InitGameDataExternal(); // returns clib's errcode
    HErr InitGameDataInternal();
    
    HErr Run();
    HErr RunSetup();

    static void AllegroExitHandler();
    static void WindowCloseHandler();

    //-------------------------------------------------------------------------
    // A copy of original engine init functions
    //
    //void change_to_directory_of_file(LPCWSTR fileName);
    int engine_init_allegro();
    void winclosehook();
    void engine_setup_window();
    int engine_check_run_setup(int argc,char*argv[]);
    void engine_force_window();
    
    
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
    void    OverrideSetup();

    // The only instance of the AGS Engine
    static CAGSEngine   *_theEngine;
    AGSPlatformDriver   *_platform;
    // EIP = Extended Instruction Pointer; here it is simply an integer, which
    // stores numerical id of last "checkpoint" in program execution
    int32_t             _eip;
    int32_t             _eipGuiNum;
    int32_t             _eipGuiObj;
    CString             _appExeName;
    CString             _appDirectory;
    CString             _gameDataFileName;
    CString             _gameDataDirectory;
    CEngineSetup        *_theSetup;
    CString             _configFileDefName;
    CString             _configFileName;
    bool                _ignoreConfigFile;
    bool                _mustChangeToGameDataDirectory;

    bool                _mustRunSetup;

    HWND                _hAllegroWnd;
    int                 _AllegroErrNo;

    CAGSGame            *_theGame;
    bool                _properExit;
    bool                _wantExit;
    bool                _abortEngine;
    bool                _checkDynamicSpritesAtExit;

};

} // namespace Core
} // namespace Engine
} // namespace AGS

#endif // __AGS_EE_CORE__AGSENGINE_H
