
#if !defined(LINUX_VERSION) && !defined(MAC_VERSION)
#include <process.h>
#endif
#if defined(WINDOWS_VERSION)
#include <new.h>
#endif
#include "Common/ac/common.h"
#include "Common/core/err.h"
#include "Common/core/out.h"
#include "Common/util/string.h"
#include "Common/util/file.h"
#include "Common/util/filestream.h"
#include "Common/util/path.h"
#include "Common/util/wgt2allg.h"
#include "Engine/ac/game.h"
#include "Engine/ac/route_finder.h"
#include "Engine/core/agsengine.h"
#include "Engine/core/engine_setup.h"
#include "Engine/core/_system.h"
#include "Engine/debug/agseditordebugger.h"
#include "Engine/game/agsgame.h"
#include "Engine/game/assets_manager.h"
#include "Engine/main/game_start.h"
#include "Engine/main/graphics_mode.h"
#include "Engine/util/inifile.h"
#include "Engine/platform/base/agsplatformdriver.h"

// FIXME! this should not be used like that
#define RETURN_CONTINUE 1

#ifdef NO_MP3_PLAYER
#define SPECIAL_VERSION "NMP"
#else
#define SPECIAL_VERSION ""
#endif

// Version and build numbers
#define AC_VERSION_TEXT "3.21 "
#define ACI_VERSION_TEXT "3.21.1115"SPECIAL_VERSION
// this needs to be updated if the "play" struct changes
#define LOWEST_SGVER_COMPAT "3.20.1103"SPECIAL_VERSION


#ifdef USE_CUSTOM_EXCEPTION_HANDLER
// This is from 'minidump' unit
extern int CustomExceptionHandler (LPEXCEPTION_POINTERS exinfo);
extern int miniDumpResultCode;
extern EXCEPTION_RECORD excinfo;
// This is from 'debug' unit
extern const char *get_cur_script(int numberOfLinesOfCallStack);
// Somewhere from allegro
extern int atexit(void (*pfn)());
#endif // USE_CUSTOM_EXCEPTION_HANDLER

// from allegro
extern "C" HWND allegro_wnd;


namespace AGS
{
namespace Engine
{
namespace Core
{

// Namespace aliases
namespace Path = AGS::Common::Util::Path;
namespace Out  = AGS::Common::Core::Out; 

// Using-declarations
using AGS::Common::Core::CErrorHandle;
using AGS::Common::Util::CFile;
using AGS::Common::Util::CFileStream;
using AGS::Engine::Util::CINIFile;
namespace Err = AGS::Common::Core::Err;

CAGSEngine *CAGSEngine::_theEngine = NULL;

CAGSEngine::~CAGSEngine()
{
    if (_theGame)   delete _theGame;
}

HErr CAGSEngine::CreateInstance()
{
    if (_theEngine)
    {
        // TODO: more appropriate way to signal error here (assert?)
        Out::Warn("WTF?? engine cannot be created for the second time!");
    }
    else
    {
        _theEngine = new CAGSEngine();
    }

    return Err::Nil();
}

CAGSEngine *CAGSEngine::GetInstance()
{
    return _theEngine;
}

void CAGSEngine::DestroyInstance()
{
    if (_theEngine)
    {
        delete _theEngine;
    }
    _theEngine = NULL;
}

/* static */ CString CAGSEngine::GetEngineVersion()
{
    return CString(ACI_VERSION_TEXT);
}

/* static */ void CAGSEngine::SetEIP(int32_t eip)
{
    _theEngine->_eip = eip;
}

/* static */ int32_t CAGSEngine::GetEIP()
{
    return _theEngine->_eip;
}

/* static */ const CCmdArgs &CAGSEngine::GetCmdArgs()
{
    return _theEngine->_cmdArgs;
}

/* static */ /*const*/ CEngineSetup &CAGSEngine::GetSetup()
{
    return _theEngine->_theSetup;
}

/* static */ CAssetsManager *CAGSEngine::GetAssetsManager()
{
    return _theEngine->_theAssetsMgr;
}

/* static */ CAGSGame *CAGSEngine::GetGame()
{
    return _theEngine->_theGame;
}

HErr CAGSEngine::StartUpAndRun(const CCmdArgs &cmdargs)
{
    HErr err = StartUp(cmdargs);
    if (!err->IsNil())
    {
        return err;
    }

    err = _RunLoop();
    return err;
}

HErr CAGSEngine::StartUp(const CCmdArgs &cmdargs)
{
    //-----------------------------------------------------
    // 1. Construct basic components.
    //
    _cmdArgs    = cmdargs;
    _platform   = AGSPlatformDriver::GetDriver();
    //// [IKM] FIXME: 
    //// I don't really like this, game should be created only after system init;
    //// but game data is set in ProcessCmdLine(); probably should be changed later
    //_theGame    = new CAGSGame();

    //-----------------------------------------------------
    // Check command line arguments and make preliminary
    // setup.
    //
    HErr err = ProcessCmdArgs();
    if (!err->IsNil())
    {
        return err;
    }

    //-----------------------------------------------------
    // 2. Set up an out-of-memory handler.
    //
#if defined(WINDOWS_VERSION)
    _set_new_handler(MallocFailHandler);
    _set_new_mode(1);
    // [IKM] I do not know where 7000 comes from, but why the heck not
    _emergencyWorkingSpace = (char*)malloc(7000);
#endif

    //-----------------------------------------------------
    // 3. Run main start-up procedure
    //
#ifndef USE_CUSTOM_EXCEPTION_HANDLER
    _theSetup.DisableExceptionHandling = 1;
#endif
    if (_theSetup.DisableExceptionHandling)
    {
        err = _StartUp();
    }
    else
    {
        err = _StartUpWithExceptionHandling();
    }

    return err;
}

HErr CAGSEngine::RunLoop()
{
    HErr err;
    if (_theSetup.DisableExceptionHandling)
    {
        err = _RunLoop();
    }
    else
    {
        err = _RunLoopWithExceptionHandling();
    }

    return err;
}

HErr CAGSEngine::Run()
{
    HErr err;
    if (_theSetup.DisableExceptionHandling)
    {
        err = _Run();
    }
    else
    {
        err = _RunWithExceptionHandling();
    }

    return err;
}

HErr CAGSEngine::_StartUp()
{
    HErr err;

    //-----------------------------------------------------
    // 4. Basic initialization
    //
    SetEIP(-999);

    // [IKM] For some bizzare reason this function is a part of routefnd module;
    // I cannot just move it here, because it references certain variables from
    // that module and so probably somehow related to it, in a logical or maybe
    // philosophical way, I just cannot figure out what at the time being.
    print_welcome_text(AC_VERSION_TEXT,ACI_VERSION_TEXT);

    // [IKM] FIXME: what's this?
    /*
    if ((argc>1) && (argv[1][1]=='?'))
        return 0;
    */

    if (!CheckMemory())
    {
        return Err::FromCode(EXIT_NORMAL);
    }

    Out::Notify("***** ENGINE STARTUP");

    //-----------------------------------------------------
    // 5. Configure Engine work
    //
    CINIFile ini;
    // Don't read in the standard config file if disabled,
    if (!_ignoreConfigFile)
    {
        // Find and read configuration file, store the key-value pairs
        err = ReadConfigFile(&ini);
    }
    if (!err->IsNil())
    {
        return err;
    }

    err = ConfigureEngine(&ini);
    if (!err->IsNil())
    {
        return err;
    }

    //-----------------------------------------------------
    // 6. Init Allegro and create application window
    //
    err = InitAllegro();
    if (!err->IsNil())
    {
        return err;
    }

    err = InitWindow();
    if (!err->IsNil())
    {
        return err;
    }

    //-----------------------------------------------------
    // 7. Run setup if must
    //
    SetEIP(-196);
    if (_mustRunSetup)
    {
        err = RunSetup();
        if (!err->IsNil())
        {
            return err;
        }
    }

    //-----------------------------------------------------

    if ((_theSetup.DebugFlags & (~DBG_DEBUGMODE)) >0) {
        _platform->DisplayAlert("Engine debugging enabled.\n"
            "\nNOTE: You have selected to enable one or more engine debugging options.\n"
            "These options cause many parts of the game to behave abnormally, and you\n"
            "may not see the game as you are used to it. The point is to test whether\n"
            "the engine passes a point where it is crashing on you normally.\n"
            "[Debug flags enabled: 0x%02X]\n"
            "Press a key to continue.\n",_theSetup.DebugFlags);
    }

    SetEIP(-195);

    //-----------------------------------------------------
    // 8. Create Engine components
    //
    err = CreateComponents();
    if (!err->IsNil())
    {
        return err;
    }
    
    //-----------------------------------------------------
    // 9. Create and prepare game
    //
    err = CreateGame();
    if (!err->IsNil())
    {
        return err;
    }

    return Err::Nil();
}

HErr CAGSEngine::_RunLoop()
{
    // Main engine loop
    while(true/*sort of...*/)
    {
        HErr err = Run();
        if (!err->IsNil())
        {
            return err;
        }
    }

    return Err::Nil();
}

HErr CAGSEngine::_Run()
{
    return Err::Nil();
}

HErr CAGSEngine::_StartUpWithExceptionHandling()
{
#ifdef USE_CUSTOM_EXCEPTION_HANDLER
    __try 
    {
#endif
        return _StartUp();
#ifdef USE_CUSTOM_EXCEPTION_HANDLER
    }
    __except (CustomExceptionHandler ( GetExceptionInformation() )) 
    {
        OnException();
    }
    return Err::FromCode(EXIT_CRASH);
#endif
}

HErr CAGSEngine::_RunLoopWithExceptionHandling()
{
#ifdef USE_CUSTOM_EXCEPTION_HANDLER
    __try 
    {
#endif
        return _RunLoop();
#ifdef USE_CUSTOM_EXCEPTION_HANDLER
    }
    __except (CustomExceptionHandler ( GetExceptionInformation() )) 
    {
        OnException();
    }
    return Err::FromCode(EXIT_CRASH);
#endif
}

HErr CAGSEngine::_RunWithExceptionHandling()
{
#ifdef USE_CUSTOM_EXCEPTION_HANDLER
    __try 
    {
#endif
        return _Run();
#ifdef USE_CUSTOM_EXCEPTION_HANDLER
    }
    __except (CustomExceptionHandler ( GetExceptionInformation() )) 
    {
        OnException();
    }
    return Err::FromCode(EXIT_CRASH);
#endif
}

#ifdef USE_CUSTOM_EXCEPTION_HANDLER
void CAGSEngine::OnException()
{
    CString tempmsg; // what is it for?
    CString msg;
    msg.Format("An exception 0x%X occurred in ACWIN.EXE at EIP = 0x%08X %s; program pointer is %+d, ACI version " ACI_VERSION_TEXT ", gtags (%d,%d)\n\n"
        "AGS cannot continue, this exception was fatal. Please note down the numbers above, remember what you were doing at the time and post the details on the AGS Technical Forum.\n\n%s\n\n"
        "Most versions of Windows allow you to press Ctrl+C now to copy this entire message to the clipboard for easy reporting.\n\n%s (code %d)",
        excinfo.ExceptionCode, excinfo.ExceptionAddress, tempmsg.GetCStr(), _eip, _eipGuiNum, _eipGuiObj, get_cur_script(5),
        (miniDumpResultCode == 0) ? "An error file CrashInfo.dmp has been created. You may be asked to upload this file when reporting this problem on the AGS Forums." : 
        "Unable to create an error dump file.", miniDumpResultCode);
    MessageBoxA(_hAllegroWnd, msg.GetCStr(), "Illegal exception", MB_ICONSTOP | MB_OK);
    _properExit = true;
}
#endif

HErr CAGSEngine::ProcessCmdArgs()
{
    int force_window        = 0;
    int force_letterbox     = 0;
    int force_16bit         = 0;
    int debug_15bit_mode    = 0;
    int debug_24bit_mode    = 0;
    int convert_16bit_bgr   = 0;
    int display_fps         = 0;
    int override_start_room = 0;
    bool just_register_game   = false;
    bool just_unregister_game = false;
    int editor_debugging_enabled = 0;
    CString return_to_roomedit;
    CString return_to_room;
    CString load_savegame_onstartup;
    CString editor_debugger_instance_token;

    //
    // Parse order independent options
    //
    //if (_cmdArgs.ContainsArgKey("-updatereg"))           _theSetup.DebugFlags |= DBG_REGONLY;
    if (_cmdArgs.ContainsArgKey("-hicolor"))             force_16bit = 1;        
    if (_cmdArgs.ContainsArgKey("-letterbox"))           force_letterbox = 1;        
    if (_cmdArgs.ContainsArgKey("-record"))              _theGame->GetGameState().recording = 1;        
    if (_cmdArgs.ContainsArgKey("-playback"))            _theGame->GetGameState().playback = 1;        
    if (_cmdArgs.ContainsArgKey("--setup"))              _mustRunSetup = true;
    if (_cmdArgs.ContainsArgKey("--15bit"))              debug_15bit_mode = 1;
    if (_cmdArgs.ContainsArgKey("--24bit"))              debug_24bit_mode = 1;
    if (_cmdArgs.ContainsArgKey("--fps"))                display_fps = 2;
    
    if (_cmdArgs.ContainsArgKey("-registergame"))        just_register_game = true;
    if (_cmdArgs.ContainsArgKey("-unregistergame"))      just_unregister_game = true;    

    //
    // Parse order dependent options
    //
    _appExeName = _cmdArgs[0];

    int _dataFileArgV = 0;
    for (int i = 1; i < _cmdArgs.GetCount(); ++i) {
        if (_cmdArgs[i][1]=='?') return 0;
#ifdef _DEBUG
        if ((_cmdArgs[i].Compare("--startr") == 0) && (i < _cmdArgs.GetCount()-1)) {
            override_start_room = _cmdArgs[i+1].ToInt();
            i++;
        }
#endif
        else if ((_cmdArgs[i].Compare("--testre") == 0) && (i < _cmdArgs.GetCount()-2)) {
            return_to_roomedit  = _cmdArgs[i+1];
            return_to_room      = _cmdArgs[i+2];
            i+=2;
        }
        else if ((_cmdArgs[i].Compare("-loadsavedgame") == 0) && (_cmdArgs.GetCount() > i + 1))
        {
            load_savegame_onstartup = _cmdArgs[i + 1];
            i++;
        }
        else if ((_cmdArgs[i].Compare("--enabledebugger") == 0) && (_cmdArgs.GetCount() > i + 1))
        {
            editor_debugger_instance_token = _cmdArgs[i + 1];
            editor_debugging_enabled = 1;
            force_window = 1;
            i++;
        }
        else if (_cmdArgs[i].Compare("--takeover")==0) {
            if (_cmdArgs.GetCount() < i+2)
                break;
            _theGame->GetGameState().takeover_data = _cmdArgs[i + 1].ToInt();
            strncpy (_theGame->GetGameState().takeover_from, _cmdArgs[i + 2].GetCStr(), 49);
            _theGame->GetGameState().takeover_from[49] = 0;
            i += 2;
        }
        else if (_cmdArgs[i][0]!='-') _dataFileArgV=i;
    }

    if ((!load_savegame_onstartup.IsEmpty()) && (!_cmdArgs[0].IsEmpty()))
    {
        // When launched by double-clicking a save game file, the curdir will
        // be the save game folder unless we correct it
        Path::SetCurrentDirectory(Path::GetParentPath(_cmdArgs[0]));
    }

    _appDirectory = Path::GetCurrentDirectory();

    //if (change_to_game_dir == 1)  {
    if (_dataFileArgV > 0) {
        // If launched by double-clicking .AGS file, change to that
        // folder; else change to this exe's folder
        Path::SetCurrentDirectory(Path::GetParentPath(_cmdArgs[_dataFileArgV]));        
    }

    //_gameDataFileName = _cmdArgs[datafile_argv];
    //_gameDataDirectory = Path::GetCurrentDirectory();

    return Err::Nil();
}

bool CAGSEngine::CheckMemory()
{
    Out::Notify("Checking memory");

    char*memcheck=(char*)malloc(4000000);
    if (memcheck==NULL) {
        _platform->DisplayAlert("There is not enough memory available to run this game. You need 4 Mb free\n"
            "extended memory to run the game.\n"
            "If you are running from Windows, check the 'DPMI memory' setting on the DOS box\n"
            "properties.\n");
        return false;
    }
    free(memcheck);
    // TODO:
    //
    //unlink (replayTempFile);
    //
    return true;
}

HErr CAGSEngine::ReadConfigFile(CINIFile *ini_file) {

    Out::Notify("Reading config file");
    SetEIP(-200);

    // Try current directory for config first; else try exe dir
    _configFileDefName = "acsetup.cfg";
    _configFileName = _configFileDefName;

    CFile *f = CFile::Open(_configFileName, "rb");
    
    if (f == NULL) {

        CString conf_file = _appExeName;
        conf_file = Path::FixFileName(conf_file);

        CINIFile::getdirec(_configFileName, &conf_file);
        //    printf("Using config: '%s'\n",conffilebuf);
        _configFileName = conf_file;
    }
    else {
        delete f;

        // put the full path, or it gets written back to the Windows folder
        _configFileName = Path::GetCurrentDirectory();
        _configFileName.AppendCStr("\\acsetup.cfg");
        _configFileName = Path::FixFileName(_configFileName);
    }

    CFileStream *fs = CFileStream::Open(_configFileName, "r");
    if (fs)
    {
        ini_file->ReadAsTree(fs);
        delete fs;
    }

    return Err::Nil();
}

HErr CAGSEngine::ConfigureEngine(CKeyValueTree *kv_tree)
{
    _theSetup.SetDefaults();
    if (kv_tree)
    {
        _theSetup.ReadFromTree(kv_tree);
    }
    _theSetup.OverrideByCmdArgs(_cmdArgs);
    _theSetup.OverrideByPlatform();
    return Err::Nil();
}

HErr CAGSEngine::InitAllegro()
{
    Out::Notify("Initializing allegro");
    SetEIP(-199);

    set_uformat(U_ASCII);

    // Initialize allegro
    if (install_allegro(SYSTEM_AUTODETECT,&_AllegroErrNo,atexit)) {
        _platform->DisplayAlert(
#ifdef WINDOWS_VERSION
            "Unable to initialize graphics subsystem. Make sure you have DirectX 5 or above installed."
#else
            "Unknown error initializing graphics subsystem."
#endif
            );
        return Err::FromCode(EXIT_NORMAL);
    }

    return Err::Nil();
}

HErr CAGSEngine::InitWindow()
{
    Out::Notify("Setting up window");
    SetEIP(-198);

#if (ALLEGRO_DATE > 19990103)
    set_window_title("Adventure Game Studio");
#if (ALLEGRO_DATE > 20021115)
    set_close_button_callback (WindowCloseHandler);
#else
    set_window_close_hook (WindowCloseHandler);
#endif
#endif

    SetEIP(-197);
    _platform->SetGameWindowIcon();

    return Err::Nil();
}

HErr CAGSEngine::RunSetup()
{
    Out::Notify("Running Setup");

    if (!_platform->RunSetup())
    {
        return Err::FromCode(EXIT_NORMAL);
    }

#ifndef WINDOWS_VERSION
#define _spawnl spawnl
#define _P_OVERLAY P_OVERLAY
#endif
    // Just re-reading the config file seems to cause a caching
    // problem on Win9x, so let's restart the process.
    allegro_exit();
    CString quoted_path;
    quoted_path.Format("\"%s\"", _appExeName);
    _spawnl (_P_OVERLAY, _appExeName.GetCStr(), quoted_path.GetCStr(), NULL);
    //read_config_file(argv[0]);

    return Err::Nil();
}

HErr CAGSEngine::CreateComponents()
{
    //-----------------------------------------------------
    // Game Assets should be initialized first; if there's
    // not game data found there's no sense in initializing
    // other components.
    //
    _theAssetsMgr = new CAssetsManager(_cmdArgs[_dataFileArgV], Path::GetCurrentDirectory());
    HErr err = _theAssetsMgr->RegisterGameData();
    if (!err->IsNil()) {
        return err;
    }

    _theSystem = new CSystem();
    err = _theSystem->Initialize();
    if (!err->IsNil()) {
        return err;
    }

    // TODO!!
    /*-------------------------------*/
    engine_init_screen_settings();
    int res = engine_init_graphics_mode();
    if (res != RETURN_CONTINUE) {
        return Err::FromCode(res);
    }
    engine_prepare_screen();
    engine_setup_screen();
    /*-------------------------------*/

    err = CreateFontRenderers();
    if (!err->IsNil()) {
        return err;
    }

    SetEIP(-183);

    Out::Notify("Install timer");
    install_timer(); // allegro

    SetEIP(-10);

    Out::Notify("Install exit handler");
    atexit(AllegroExitHandler); // allegro

    Out::Notify("Initialize path finder library");
    init_pathfinder(); // route_finder

    SetEIP(-179);

    engine_init_modxm_player(); //???

    res = engine_init_gfx_filters();
    if (res != RETURN_CONTINUE) {
        return Err::FromCode(res);
    }

    engine_init_gfx_driver();
    engine_post_init_gfx_driver();

    _platform->PostAllegroInit((_theSetup.Windowed > 0) ? true : false);

    engine_set_gfx_driver_callbacks();
    engine_set_color_conversions();

    _theSystem->GetScreen()->SetMultitasking(false);

    return Err::Nil();
}

HErr CAGSEngine::CreateFontRenderers()
{
    SetEIP(-192);

    Out::Notify("Initializing TTF renderer");
    init_font_renderer(); // in common/ac_fonts

    SetEIP(-188);

    return Err::Nil();
}

HErr CAGSEngine::CreateGame()
{
    _theGame = new CAGSGame();
    HErr err = _theGame->Initialize();

    // TODO: do this somehow else
    if (_theSetup.Digicard == DIGI_NONE) {
        // disable speech and music if no digital sound
        // therefore the MIDI soundtrack will be used if present,
        // and the voice mode should not go to Voice Only
        _theGame->GetGameState().want_speech = -2;
        //_theGame->GetGameState().seperate_music_lib = 0;
        _theAssetsMgr->_seperateMusicLib = 0;
    }

    SetEIP(-185);

    SetEIP(-182);

    Out::Notify("Initialize gfx");
    _platform->InitialiseAbufAtStartup();

    LOCK_VARIABLE(timerloop);
    LOCK_FUNCTION(dj_timer_handler);
    set_game_speed(40);

    SetEIP(-20);
    //thisroom.allocall();
    SetEIP(-19);
    //setup_sierra_interface();   // take this out later

    int res = engine_load_game_data();
    if (res != RETURN_CONTINUE) {
        return Err::FromCode(res);
    }

    res = engine_check_register_game();
    if (res != RETURN_CONTINUE) {
        return Err::FromCode(res);
    }

    engine_init_title();

    SetEIP(-189);

    engine_init_directories();

    SetEIP(-178);

    res = engine_check_disk_space();
    if (res != RETURN_CONTINUE) {
        return Err::FromCode(res);
    }

    // [IKM] I do not really understand why is this checked only now;
    // should not it be checked right after fonts initialization?
    res = engine_check_fonts();
    if (res != RETURN_CONTINUE) {
        return Err::FromCode(res);
    }

    engine_init_rand();

    engine_show_preload();

    res = engine_init_sprites();
    if (res != RETURN_CONTINUE) {
        return Err::FromCode(res);
    }

    engine_init_game_settings();

    engine_prepare_to_start_game();

    initialize_start_and_play_game(_overrideStartRoom, _loadSaveGameOnStartup);

    _updateMP3ThreadRunning = false;
    quit("|bye!");

    return Err::Nil();
}

#if defined(WINDOWS_VERSION)
/* static */ int CAGSEngine::MallocFailHandler(size_t amountwanted) {
    // Drop the ballast to ensure some free memory space
    free(_theEngine->_emergencyWorkingSpace);

    char tempmsg[100];
    sprintf(tempmsg,"Out of memory: failed to allocate %ld bytes (at PP=%d)",amountwanted, _theEngine->GetEIP());
    quit(tempmsg);
    return 0;
}
#endif

/* static */ void CAGSEngine::AllegroExitHandler() {
    if (!_theEngine->_properExit) {
        CString msg;
        msg.Format("\nError: the program has exited without requesting it.\n"
            "Program pointer: %+03d  (write this number down), ACI version " ACI_VERSION_TEXT "\n"
            "If you see a list of numbers above, please write them down and contact\n"
            "Chris Jones. Otherwise, note down any other information displayed.\n",
            _theEngine->_eip);
        _theEngine->_platform->DisplayAlert(msg.GetCStr());
    }
}

/* static */ void CAGSEngine::WindowCloseHandler()
{
  _theEngine->_wantExit = true;
  _theEngine->_abortEngine = true;
  _theEngine->_checkDynamicSpritesAtExit = true;
}

void CAGSEngine::engine_init_rand()
{
#ifdef ______NOT_NOW
    _theGame->GetGameState().randseed = time(NULL);
    srand (_theGame->GetGameState().randseed);
#endif
}

int CAGSEngine::engine_load_game_data()
{
#ifdef ______NOT_NOW
    Out::Notify("Load game data");

    our_eip=-17;
    int ee=load_game_file();
    if (ee != 0) {
        proper_exit=1;
        _platform->FinishedUsingGraphicsMode();

        if (ee==-1)
            _platform->DisplayAlert("Main game file not found. This may be from a different AGS version, or the file may have got corrupted.\n");
        else if (ee==-2)
            _platform->DisplayAlert("Invalid file format. The file may be corrupt, or from a different\n"
            "version of AGS.\nThis engine can only run games made with AGS 2.5 or later.\n");
        else if (ee==-3)
            _platform->DisplayAlert("Script link failed: %s\n",ccErrorString);
        return EXIT_NORMAL;
    }
#endif

    return RETURN_CONTINUE;
}

int CAGSEngine::engine_check_register_game()
{
#ifdef ______NOT_NOW
    if (justRegisterGame) 
    {
        _platform->RegisterGameWithGameExplorer();
        proper_exit = 1;
        return EXIT_NORMAL;
    }

    if (justUnRegisterGame) 
    {
        _platform->UnRegisterGameWithGameExplorer();
        proper_exit = 1;
        return EXIT_NORMAL;
    }
#endif

    return RETURN_CONTINUE;
}

void CAGSEngine::engine_init_title()
{
#ifdef ______NOT_NOW
    //_platform->DisplayAlert("loaded game");
    our_eip=-91;
#if (ALLEGRO_DATE > 19990103)
    set_window_title(game.gamename);
#endif

    Out::Notify(game.gamename);
#endif
}

void CAGSEngine::engine_init_directories()
{
#ifdef ______NOT_NOW
    if (file_exists("Compiled", FA_ARCH | FA_DIREC, NULL))
    {
        // running in debugger
        use_compiled_folder_as_current_dir = 1;
        // don't redirect to the game exe folder (_Debug)
        _theSetup.data_files_dir = ".";
    }

    if (game.saveGameFolderName[0] != 0)
    {
        char newDirBuffer[MAX_PATH];
        sprintf(newDirBuffer, "$MYDOCS$/%s", game.saveGameFolderName);
        Game_SetSaveGameDirectory(newDirBuffer);
    }
    else if (use_compiled_folder_as_current_dir)
    {
        Game_SetSaveGameDirectory("Compiled");
    }
#endif
}

int CAGSEngine::check_write_access() {

#ifdef ______NOT_NOW
  if (_platform->GetDiskFreeSpaceMB() < 2)
    return 0;

  SetEIP(-1895);

  // The Save Game Dir is the only place that we should write to
  char tempPath[MAX_PATH];
  sprintf(tempPath, "%s""tmptest.tmp", saveGameDirectory);
  FILE *yy = fopen(tempPath, "wb");
  if (yy == NULL)
    return 0;

  SetEIP(-1896);

  fwrite("just to test the drive free space", 30, 1, yy);
  fclose(yy);

  SetEIP(-1897);

  if (unlink(tempPath))
    return 0;
#endif
  return 1;
}

int CAGSEngine::engine_check_disk_space()
{
#ifdef ______NOT_NOW
    Out::Notify("Checking for disk space");

    //init_language_text("en");
    if (check_write_access()==0) {
#if defined(IOS_VERSION)
        _platform->DisplayAlert("Unable to write to the current directory. Make sure write permissions are"
            " set for the game directory.\n");
#else
        _platform->DisplayAlert("Unable to write to the current directory. Do not run this game off a\n"
            "network or CD-ROM drive. Also check drive free space (you need 1 Mb free).\n");
#endif
        proper_exit = 1;
        return EXIT_NORMAL; 
    }
#endif

    return RETURN_CONTINUE;
}

// [IKM] I have a feeling this should be merged with engine_init_fonts
int CAGSEngine::engine_check_fonts()
{
#ifdef ______NOT_NOW
    if (fontRenderers[0] == NULL) 
    {
        _platform->DisplayAlert("No fonts found. If you're trying to run the game from the Debug directory, this is not supported. Use the Build EXE command to create an executable in the Compiled folder.");
        proper_exit = 1;
        return EXIT_NORMAL;
    }
#endif

    return RETURN_CONTINUE;
}

void CAGSEngine::engine_init_modxm_player()
{
#ifdef ______NOT_NOW
#ifndef PSP_NO_MOD_PLAYBACK
    if (game.options[OPT_NOMODMUSIC])
        opts.mod_player = 0;

    if (opts.mod_player) {
        Out::Notify("Initializing MOD/XM player");

        if (init_mod_player(NUM_MOD_DIGI_VOICES) < 0) {
            _platform->DisplayAlert("Warning: install_mod: MOD player failed to initialize.");
            opts.mod_player=0;
        }
    }
#else
    opts.mod_player = 0;
    Out::Notify("Compiled without MOD/XM player");
#endif

    _platform->WriteConsole("Checking sound inits.\n");
    if (opts.mod_player) reserve_voices(16,-1);
    // maybe this line will solve the sound volume?
    // [IKM] does this refer to install_timer or set_volume_per_voice?
#if ALLEGRO_DATE > 19991010
    set_volume_per_voice(1);
#endif
    //set_volume(255,-1);
#endif
}

void CAGSEngine::show_preload () {
#ifdef ______NOT_NOW
    // ** Do the preload graphic if available
    color temppal[256];
    block splashsc = load_pcx("preload.pcx",temppal);
    if (splashsc != NULL) {
        if (bitmap_color_depth(splashsc) == 8)
            wsetpalette(0,255,temppal);
        block tsc = create_bitmap_ex(bitmap_color_depth(screen),splashsc->w,splashsc->h);
        blit(splashsc,tsc,0,0,0,0,tsc->w,tsc->h);
        clear(screen);
        stretch_sprite(screen, tsc, 0, 0, scrnwid,scrnhit);

        gfxDriver->ClearDrawList();

        if (!gfxDriver->UsesMemoryBackBuffer())
        {
            IDriverDependantBitmap *ddb = gfxDriver->CreateDDBFromBitmap(screen, false, true);
            gfxDriver->DrawSprite(0, 0, ddb);
            render_to_screen(screen, 0, 0);
            gfxDriver->DestroyDDB(ddb);
        }
        else
            render_to_screen(screen, 0, 0);

        wfreeblock(splashsc);
        wfreeblock(tsc);
        _platform->Delay(500);
    }
#endif
}

void CAGSEngine::engine_show_preload()
{
    Out::Notify("Check for preload image");

    show_preload ();
}

int CAGSEngine::engine_init_sprites()
{
#ifdef ______NOT_NOW
    Out::Notify("Initialize sprites");

    if (spriteset.initFile ("acsprset.spr")) 
    {
        _platform->FinishedUsingGraphicsMode();
        allegro_exit();
        proper_exit=1;
        _platform->DisplayAlert("Could not load sprite set file ACSPRSET.SPR\n"
            "This means that the file is missing or there is not enough free\n"
            "system memory to load the file.\n");
        return EXIT_NORMAL;
    }
#endif
    return RETURN_CONTINUE;
}

void CAGSEngine::engine_setup_screen()
{
#ifdef ______NOT_NOW
    Out::Notify("Set up screen");

    virtual_screen=create_bitmap_ex(final_col_dep,scrnwid,scrnhit);
    clear(virtual_screen);
    gfxDriver->SetMemoryBackBuffer(virtual_screen);
    //  ignore_mouseoff_bitmap = virtual_screen;
    abuf=screen;
    our_eip=-7;

    for (int ee = 0; ee < MAX_INIT_SPR + game.numcharacters; ee++)
        actsps[ee] = NULL;
#endif
}

void CAGSEngine::init_game_settings() {

#ifdef ______NOT_NOW
    int ee;

    for (ee=0;ee<256;ee++) {
        if (game.paluses[ee]!=PAL_BACKGROUND)
            palette[ee]=game.defpal[ee];
    }

    if (game.options[OPT_NOSCALEFNT]) wtext_multiply=1;

    for (ee = 0; ee < game.numcursors; ee++) 
    {
        // The cursor graphics are assigned to mousecurs[] and so cannot
        // be removed from memory
        if (game.mcurs[ee].pic >= 0)
            spriteset.precache (game.mcurs[ee].pic);

        // just in case they typed an invalid view number in the editor
        if (game.mcurs[ee].view >= game.numviews)
            game.mcurs[ee].view = -1;

        if (game.mcurs[ee].view >= 0)
            precache_view (game.mcurs[ee].view);
    }
    // may as well preload the character gfx
    if (playerchar->view >= 0)
        precache_view (playerchar->view);

    for (ee = 0; ee < MAX_INIT_SPR; ee++)
        objcache[ee].image = NULL;

    /*  dummygui.guiId = -1;
    dummyguicontrol.guin = -1;
    dummyguicontrol.objn = -1;*/

    our_eip=-6;
    //  game.chars[0].talkview=4;
    //init_language_text(game.langcodes[0]);

    for (ee = 0; ee < MAX_INIT_SPR; ee++) {
        scrObj[ee].id = ee;
        scrObj[ee].obj = NULL;
    }

    for (ee=0;ee<game.numcharacters;ee++) {
        memset(&game.chars[ee].inv[0],0,MAX_INV*sizeof(short));
        game.chars[ee].activeinv=-1;
        game.chars[ee].following=-1;
        game.chars[ee].followinfo=97 | (10 << 8);
        game.chars[ee].idletime=20;  // can be overridden later with SetIdle or summink
        game.chars[ee].idleleft=game.chars[ee].idletime;
        game.chars[ee].transparency = 0;
        game.chars[ee].baseline = -1;
        game.chars[ee].walkwaitcounter = 0;
        game.chars[ee].z = 0;
        charextra[ee].xwas = INVALID_X;
        charextra[ee].zoom = 100;
        if (game.chars[ee].view >= 0) {
            // set initial loop to 0
            game.chars[ee].loop = 0;
            // or to 1 if they don't have up/down frames
            if (views[game.chars[ee].view].loops[0].numFrames < 1)
                game.chars[ee].loop = 1;
        }
        charextra[ee].process_idle_this_time = 0;
        charextra[ee].invorder_count = 0;
        charextra[ee].slow_move_counter = 0;
        charextra[ee].animwait = 0;
    }
    // multiply up gui positions
    guibg = (block*)malloc(sizeof(block) * game.numgui);
    guibgbmp = (IDriverDependantBitmap**)malloc(sizeof(IDriverDependantBitmap*) * game.numgui);
    for (ee=0;ee<game.numgui;ee++) {
        guibgbmp[ee] = NULL;
        GUIMain*cgp=&guis[ee];
        guibg[ee] = create_bitmap_ex (final_col_dep, cgp->wid, cgp->hit);
        guibg[ee] = gfxDriver->ConvertBitmapToSupportedColourDepth(guibg[ee]);
    }

    our_eip=-5;
    for (ee=0;ee<game.numinvitems;ee++) {
        if (game.invinfo[ee].flags & IFLG_STARTWITH) playerchar->inv[ee]=1;
        else playerchar->inv[ee]=0;
    }
    _theGame->GetGameState().score=0;
    _theGame->GetGameState().sierra_inv_color=7;
    _theGame->GetGameState().talkanim_speed = 5;
    _theGame->GetGameState().inv_item_wid = 40;
    _theGame->GetGameState().inv_item_hit = 22;
    _theGame->GetGameState().messagetime=-1;
    _theGame->GetGameState().disabled_user_interface=0;
    _theGame->GetGameState().gscript_timer=-1;
    _theGame->GetGameState().debug_mode=game.options[OPT_DEBUGMODE];
    _theGame->GetGameState().inv_top=0;
    _theGame->GetGameState().inv_numdisp=0;
    _theGame->GetGameState().obsolete_inv_numorder=0;
    _theGame->GetGameState().text_speed=15;
    _theGame->GetGameState().text_min_display_time_ms = 1000;
    _theGame->GetGameState().ignore_user_input_after_text_timeout_ms = 500;
    _theGame->GetGameState().ignore_user_input_until_time = 0;
    _theGame->GetGameState().lipsync_speed = 15;
    _theGame->GetGameState().close_mouth_speech_time = 10;
    _theGame->GetGameState().disable_antialiasing = 0;
    _theGame->GetGameState().rtint_level = 0;
    _theGame->GetGameState().rtint_light = 255;
    _theGame->GetGameState().text_speed_modifier = 0;
    _theGame->GetGameState().text_align = SCALIGN_LEFT;
    // Make the default alignment to the right with right-to-left text
    if (game.options[OPT_RIGHTLEFTWRITE])
        _theGame->GetGameState().text_align = SCALIGN_RIGHT;

    _theGame->GetGameState().speech_bubble_width = get_fixed_pixel_size(100);
    _theGame->GetGameState().bg_frame=0;
    _theGame->GetGameState().bg_frame_locked=0;
    _theGame->GetGameState().bg_anim_delay=0;
    _theGame->GetGameState().anim_background_speed = 0;
    _theGame->GetGameState().silent_midi = 0;
    _theGame->GetGameState().current_music_repeating = 0;
    _theGame->GetGameState().skip_until_char_stops = -1;
    _theGame->GetGameState().get_loc_name_last_time = -1;
    _theGame->GetGameState().get_loc_name_save_cursor = -1;
    _theGame->GetGameState().restore_cursor_mode_to = -1;
    _theGame->GetGameState().restore_cursor_image_to = -1;
    _theGame->GetGameState().ground_level_areas_disabled = 0;
    _theGame->GetGameState().next_screen_transition = -1;
    _theGame->GetGameState().temporarily_turned_off_character = -1;
    _theGame->GetGameState().inv_backwards_compatibility = 0;
    _theGame->GetGameState().gamma_adjustment = 100;
    _theGame->GetGameState().num_do_once_tokens = 0;
    _theGame->GetGameState().do_once_tokens = NULL;
    _theGame->GetGameState().music_queue_size = 0;
    _theGame->GetGameState().shakesc_length = 0;
    _theGame->GetGameState().wait_counter=0;
    _theGame->GetGameState().key_skip_wait = 0;
    _theGame->GetGameState().cur_music_number=-1;
    _theGame->GetGameState().music_repeat=1;
    _theGame->GetGameState().music_master_volume=160;
    _theGame->GetGameState().digital_master_volume = 100;
    _theGame->GetGameState().screen_flipped=0;
    _theGame->GetGameState().offsets_locked=0;
    _theGame->GetGameState().cant_skip_speech = user_to_internal_skip_speech(game.options[OPT_NOSKIPTEXT]);
    _theGame->GetGameState().sound_volume = 255;
    _theGame->GetGameState().speech_volume = 255;
    _theGame->GetGameState().normal_font = 0;
    _theGame->GetGameState().speech_font = 1;
    _theGame->GetGameState().speech_text_shadow = 16;
    _theGame->GetGameState().screen_tint = -1;
    _theGame->GetGameState().bad_parsed_word[0] = 0;
    _theGame->GetGameState().swap_portrait_side = 0;
    _theGame->GetGameState().swap_portrait_lastchar = -1;
    _theGame->GetGameState().in_conversation = 0;
    _theGame->GetGameState().skip_display = 3;
    _theGame->GetGameState().no_multiloop_repeat = 0;
    _theGame->GetGameState().in_cutscene = 0;
    _theGame->GetGameState().fast_forward = 0;
    _theGame->GetGameState().totalscore = game.totalscore;
    _theGame->GetGameState().roomscript_finished = 0;
    _theGame->GetGameState().no_textbg_when_voice = 0;
    _theGame->GetGameState().max_dialogoption_width = get_fixed_pixel_size(180);
    _theGame->GetGameState().no_hicolor_fadein = 0;
    _theGame->GetGameState().bgspeech_game_speed = 0;
    _theGame->GetGameState().bgspeech_stay_on_display = 0;
    _theGame->GetGameState().unfactor_speech_from_textlength = 0;
    _theGame->GetGameState().mp3_loop_before_end = 70;
    _theGame->GetGameState().speech_music_drop = 60;
    _theGame->GetGameState().room_changes = 0;
    _theGame->GetGameState().check_interaction_only = 0;
    _theGame->GetGameState().replay_hotkey = 318;  // Alt+R
    _theGame->GetGameState().dialog_options_x = 0;
    _theGame->GetGameState().dialog_options_y = 0;
    _theGame->GetGameState().min_dialogoption_width = 0;
    _theGame->GetGameState().disable_dialog_parser = 0;
    _theGame->GetGameState().ambient_sounds_persist = 0;
    _theGame->GetGameState().screen_is_faded_out = 0;
    _theGame->GetGameState().player_on_region = 0;
    _theGame->GetGameState().top_bar_backcolor = 8;
    _theGame->GetGameState().top_bar_textcolor = 16;
    _theGame->GetGameState().top_bar_bordercolor = 8;
    _theGame->GetGameState().top_bar_borderwidth = 1;
    _theGame->GetGameState().top_bar_ypos = 25;
    _theGame->GetGameState().top_bar_font = -1;
    _theGame->GetGameState().screenshot_width = 160;
    _theGame->GetGameState().screenshot_height = 100;
    _theGame->GetGameState().speech_text_align = SCALIGN_CENTRE;
    _theGame->GetGameState().auto_use_walkto_points = 1;
    _theGame->GetGameState().inventory_greys_out = 0;
    _theGame->GetGameState().skip_speech_specific_key = 0;
    _theGame->GetGameState().abort_key = 324;  // Alt+X
    _theGame->GetGameState().fade_to_red = 0;
    _theGame->GetGameState().fade_to_green = 0;
    _theGame->GetGameState().fade_to_blue = 0;
    _theGame->GetGameState().show_single_dialog_option = 0;
    _theGame->GetGameState().keep_screen_during_instant_transition = 0;
    _theGame->GetGameState().read_dialog_option_colour = -1;
    _theGame->GetGameState().narrator_speech = game.playercharacter;
    _theGame->GetGameState().crossfading_out_channel = 0;
    _theGame->GetGameState().speech_textwindow_gui = game.options[OPT_TWCUSTOM];
    if (_theGame->GetGameState().speech_textwindow_gui == 0)
        _theGame->GetGameState().speech_textwindow_gui = -1;
    strcpy(_theGame->GetGameState().game_name, game.gamename);
    _theGame->GetGameState().lastParserEntry[0] = 0;
    _theGame->GetGameState().follow_change_room_timer = 150;
    for (ee = 0; ee < MAX_BSCENE; ee++) 
        _theGame->GetGameState().raw_modified[ee] = 0;
    _theGame->GetGameState().game_speed_modifier = 0;
    if (_theSetup.DebugFlags & DBG_DEBUGMODE)
        _theGame->GetGameState().debug_mode = 1;
    gui_disabled_style = convert_gui_disabled_style(game.options[OPT_DISABLEOFF]);

    memset(&_theGame->GetGameState().walkable_areas_on[0],1,MAX_WALK_AREAS+1);
    memset(&_theGame->GetGameState().script_timers[0],0,MAX_TIMERS * sizeof(int));
    memset(&_theGame->GetGameState().default_audio_type_volumes[0], -1, MAX_AUDIO_TYPES * sizeof(int));

    // reset graphical script vars (they're still used by some games)
    for (ee = 0; ee < MAXGLOBALVARS; ee++) 
        _theGame->GetGameState().globalvars[ee] = 0;

    for (ee = 0; ee < MAXGLOBALSTRINGS; ee++)
        _theGame->GetGameState().globalstrings[ee][0] = 0;

    for (ee = 0; ee < MAX_SOUND_CHANNELS; ee++)
        last_sound_played[ee] = -1;

    if (_theSetup.translation)
        init_translation (_theSetup.translation);

    update_invorder();
    displayed_room = -10;
#endif
}

void CAGSEngine::engine_init_game_settings()
{
    Out::Notify("Initialize game settings");

    init_game_settings();
}

void CAGSEngine::engine_init_game_shit()
{
#ifdef ______NOT_NOW
    scsystem.width = final_scrn_wid;
    scsystem.height = final_scrn_hit;
    scsystem.coldepth = final_col_dep;
    scsystem.windowed = 0;
    scsystem.vsync = 0;
    scsystem.viewport_width = divide_down_coordinate(scrnwid);
    scsystem.viewport_height = divide_down_coordinate(scrnhit);
    strcpy(scsystem.aci_version, ACI_VERSION_TEXT);
    scsystem.os = _platform->GetSystemOSID();

    if (_theSetup.windowed)
        scsystem.windowed = 1;

#if defined(WINDOWS_VERSION) || defined(LINUX_VERSION) || defined(MAC_VERSION)
    filter->SetMouseArea(0, 0, scrnwid-1, scrnhit-1);
#else
    filter->SetMouseArea(0,0,BASEWIDTH-1,BASEHEIGHT-1);
#endif
    //  mloadwcursor("mouse.spr");
    //mousecurs[0]=spriteset[2054];
    currentcursor=0;
    our_eip=-4;
    mousey=100;  // stop icon bar popping up
    init_invalid_regions(final_scrn_hit);
    wsetscreen(virtual_screen);
    SetEIP(-41);

    gfxDriver->SetRenderOffset(get_screen_x_adjustment(virtual_screen), get_screen_y_adjustment(virtual_screen));
#endif
}

#if defined(PSP_VERSION)
// PSP: Workaround for sound stuttering. Do sound updates in its own thread.
int CAGSEngine::update_mp3_thread(SceSize args, void *argp)
{
  while (update_mp3_thread_running)
  {
    UPDATE_MP3_THREAD
    sceKernelDelayThread(1000 * 50);
  }
  return 0;
}
#elif (defined(LINUX_VERSION) && !defined(PSP_VERSION)) || defined(MAC_VERSION)
void* CAGSEngine::update_mp3_thread(void* arg)
{
  while (update_mp3_thread_running)
  {
    UPDATE_MP3_THREAD
    usleep(1000 * 50);
  }
  pthread_exit(NULL);
}
#elif defined(WINDOWS_VERSION)
DWORD WINAPI CAGSEngine::update_mp3_thread(LPVOID lpParam)
{
#ifdef ______NOT_NOW
  while (update_mp3_thread_running)
  {
    UPDATE_MP3_THREAD
    Sleep(50);
  }
#endif
  return 0;
}
#endif

void CAGSEngine::engine_start_multithreaded_audio()
{
#ifdef ______NOT_NOW
    // PSP: Initialize the sound cache.
    clear_sound_cache();

    // Create sound update thread. This is a workaround for sound stuttering.
    if (psp_audio_multithreaded)
    {
#if defined(PSP_VERSION)
        update_mp3_thread_running = true;
        SceUID thid = sceKernelCreateThread("update_mp3_thread", update_mp3_thread, 0x20, 0xFA0, THREAD_ATTR_USER, 0);
        if (thid > -1)
            thid = sceKernelStartThread(thid, 0, 0);
        else
        {
            update_mp3_thread_running = false;
            psp_audio_multithreaded = 0;
        }
#elif (defined(LINUX_VERSION) && !defined(PSP_VERSION)) || defined(MAC_VERSION)
        update_mp3_thread_running = true;
        if (pthread_create(&soundthread, NULL, update_mp3_thread, NULL) != 0)
        {
            update_mp3_thread_running = false;
            psp_audio_multithreaded = 0;
        }
#elif defined(WINDOWS_VERSION)
        update_mp3_thread_running = true;
        if (CreateThread(NULL, 0, update_mp3_thread, NULL, 0, NULL) == NULL)
        {
            update_mp3_thread_running = false;
            psp_audio_multithreaded = 0;
        }
#endif
    }
#endif
}

void CAGSEngine::engine_prepare_to_start_game()
{
#ifdef ______NOT_NOW
    Out::Notify("Prepare to start game");

    engine_init_game_shit();
    engine_start_multithreaded_audio();

#if defined(ANDROID_VERSION)
    if (psp_load_latest_savegame)
        selectLatestSavegame();
#endif
#endif
}

CAGSEngine::CAGSEngine()
    : _theSetup(*new CEngineSetup())
    , _hAllegroWnd(allegro_wnd)
{
    _eip                = 0;
    _eipGuiNum          = 0;
    _eipGuiObj          = 0;
    _ignoreConfigFile   = false;

    _mustRunSetup       = false;

    _theAssetsMgr       = NULL;
    _theGame            = NULL;

    _properExit                 = false;
    _wantExit                   = false;
    _abortEngine                = false;
    _checkDynamicSpritesAtExit  = false;
}

} // namespace Core
} // namespace Engine
} // namespace AGS
